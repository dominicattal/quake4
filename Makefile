include .env

MODDIR=$(QUAKEDIR)/$(MODNAME)

all: release

build:
	@echo "Building"
	@bash "build.sh" "Release"
	@echo "Zipping"

copy:
	@zip -j "$(MODDIR)/game000.pk4" "binary.conf" "$(BUILDDIR)/Gamex86.dll"
	@zip -r "$(MODDIR)/pak001.pk4" "def" "guis" "maps" "scripts"
	cp -r "strings" "$(MODDIR)" 

release: moddir build copy

moddir:
	@mkdir -p $(MODDIR)

env:
	@echo MODNAME=$(MODNAME)
	@echo QUAKEDIR=$(QUAKEDIR)
	@echo BUILDDIR=$(MODDIR)
	@echo MODDIR=$(BUILDDIR)


.PHONY: release debug

