PLUGIN = org.opencpn.OpenCPN.Plugin.shipdriver

all: build


build:
	flatpak-builder --force-clean app $(PLUGIN).yaml
	flatpak build-export repo app stable

install:
	flatpak install --user --reinstall -y $(CURDIR)/repo $(PLUGIN)

uninstall:
	flatpak uninstall --user $(PLUGIN)


