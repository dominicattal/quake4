BUILD=Release
BUILDDIR=/c/Users/attal/Documents/School/IT266/Win32/Release
MODDIR=/c/Users/attal/Steam/steamapps/common/Quake 4/rocket

all: release

release:
	@echo "Building"
	@bash "build.sh"
	@echo "Zipping"
	@zip -j "$(MODDIR)/game000.pk4" "$(MODDIR)/binary.conf" "$(BUILDDIR)/Gamex86.dll"
	@cp -r "def" "$(MODDIR)"
