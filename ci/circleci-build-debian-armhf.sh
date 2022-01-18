#!/usr/bin/env bash
#
# Build for Debian armhf in a docker container
#
# Copyright (c) 2021 Alec Leamas
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.

set -xe

# Create build script
#
if [ -n "$TRAVIS_BUILD_DIR" ]; then
    ci_source="$TRAVIS_BUILD_DIR"
elif [ -d ~/project ]; then
    ci_source=~/project
else
    ci_source="$(pwd)"
fi

cat > $ci_source/build.sh << "EOF"

set -x

apt -y update
apt -y install devscripts equivs wget git lsb-release

mk-build-deps /ci-source/build-deps/control
apt install -q -y ./opencpn-build-deps*deb
apt-get -q --allow-unauthenticated install -f

debian_rel=$(lsb_release -sc)

if [ "$debian_rel" = bullseye ]; then
    echo "deb http://deb.debian.org/debian bullseye-backports main" \
      >> /etc/apt/sources.list
    apt update
    apt install -y cmake/bullseye-backports
elif [ "$debian_rel" = buster ]; then
    url='https://dl.cloudsmith.io/public/alec-leamas/opencpn-plugins-stable'
    wget --no-verbose \
        $url/deb/debian/pool/buster/main/c/cm/cmake-data_3.19.3-0.1_all.deb
    wget --no-verbose \
        $url/deb/debian/pool/buster/main/c/cm/cmake_3.19.3-0.1_armhf.deb
    apt install -y ./cmake_3.19.3-0.1_armhf.deb ./cmake-data_3.19.3-0.1_all.deb
else
    echo "Unknown debian release: $debian_rel"
fi

cd /ci-source
rm -rf build-debian; mkdir build-debian; cd build-debian
cmake -DCMAKE_BUILD_TYPE=Release -DOCPN_TARGET_TUPLE="@TARGET_TUPLE@" ..
make -j $(nproc) VERBOSE=1 tarball
ldd  app/*/lib/opencpn/*.so
chown --reference=.. .
EOF

sed -i "s/@TARGET_TUPLE@/$TARGET_TUPLE/" $ci_source/build.sh


# Run script in docker image
#
if [ -n "$CI" ]; then
    sudo apt -q update
    sudo apt install qemu-user-static
fi
docker run --rm --privileged multiarch/qemu-user-static:register --reset || :
docker run --platform linux/arm/v7 --privileged \
    -e "CLOUDSMITH_STABLE_REPO=$CLOUDSMITH_STABLE_REPO" \
    -e "CLOUDSMITH_BETA_REPO=$OCPN_BETA_REPO" \
    -e "CLOUDSMITH_UNSTABLE_REPO=$CLOUDSMITH_UNSTABLE_REPO" \
    -e "CIRCLE_BUILD_NUM=$CIRCLE_BUILD_NUM" \
    -e "TRAVIS_BUILD_NUMBER=$TRAVIS_BUILD_NUMBER" \
    -v "$ci_source:/ci-source:rw" \
    -v /usr/bin/qemu-arm-static:/usr/bin/qemu-arm-static \
    arm32v7/debian:$OCPN_TARGET /bin/bash -xe /ci-source/build.sh
rm -f $ci_source/build.sh


# Install cloudsmith-cli (for upload) and cryptography (for git-push).
#
if pyenv versions &>/dev/null;  then
    pyenv versions | tr -d '*' | awk '{print $1}' | tail -1 \
        > $HOME/.python-version
fi
python3 -m pip install -q --user cloudsmith-cli cryptography

# python install scripts in ~/.local/bin, teach upload.sh to use in it's PATH:
echo 'export PATH=$PATH:$HOME/.local/bin' >> ~/.uploadrc
