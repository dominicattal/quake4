include .env

MODDIR=$(QUAKEDIR)/$(MODNAME)

all: release

release: moddir
	@echo "Building"
	@bash "build.sh" "Release"
	@echo "Zipping"
	@zip -j "$(MODDIR)/game000.pk4" "binary.conf" "$(BUILDDIR)/Gamex86.dll"
	@zip -r "$(MODDIR)/pak001.pk4" "def" "guis"

debug: moddir
	@echo "Building"
	@bash "build.sh" "Debug"
	@echo "Zipping"
	@zip -j "$(MODDIR)/game000.pk4" "binary.conf" "$(BUILDDIR)/Gamex86.dll"
	@zip -jr "$(MODDIR)/pak001.pk4" "def" "guis"

moddir:
	@mkdir -p $(MODDIR)

env:
	@echo MODNAME=$(MODNAME)
	@echo QUAKEDIR=$(QUAKEDIR)
	@echo BUILDDIR=$(MODDIR)
	@echo MODDIR=$(BUILDDIR)


.PHONY: release debug

