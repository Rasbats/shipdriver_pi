TOOLS README
============

This file collects some notes on available tool integrations

pre-commit
----------

The pre-commit project at  https://pre-commit.com/ offers a flexible
configuration of git pre-commit hooks. The template contains the file
file *.pre-commit.yaml* which is a canned configuration handling white
space fixes and, for source files, cmake-format.

The basic way to use this is to make sure that python, including pip, is
available. On Linux this is normally out of the box, other OS:s needs
separate steps outside the scope of this document.

With python/pip in place do:

  - `pip install --user pre-commit`
  - `pre-commit install`

This runs checks when doing `git commit`. Occasionally they cause
trouble; then use `git commit --no-check` instead.

CLion
-----

CLion has problems with our two-stage cmake setup -- it looks for include
paths defined by cmake, but these does not spring into existence until the
second run which takes place when doing for example `make tarball` or so.

This can be handled by creating a file called _local_config.cmake_. A typical
example:
```
    include_directories(
      libs/fast_csv_parser/fast_csv_parser
      libs/std_filesystem/include
      /usr/include/wx-3.2/
    )
```
The path to wxWidgets, here _ /usr/include/wx-3.2/_ is always required. Often
also some paths to libraries needs to be added so CLions "sees" them. When
correct the CLion build should be OK.

This file is not touched by update-templates. It's also listed in *.gitignore*
so it is not part of the git file tree and is thus not used in CI builds.
