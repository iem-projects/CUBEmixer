# Makefile for building and locally installing pd in CUBEmixer (winfried)
# note: you need only to do this if you dont want to use a already installed pd

EXT?=pd_linux

# for INSTALLDIR even if relative we need an absolute path, since configure will complain.
PWD=$(shell pwd)
LOCINSTALL_DIR=$(PWD)/../lib/pd

PDCONFIGS=-enable-alsa --enable-jack

.PHONY: install locinstall

# default build and locally install
install: locinstall

pd-build: 
	cd pd/src; autoconf; ./configure --prefix=$(LOCINSTALL_DIR) $(PDCONFIGS)
	make -C pd/src

pd-locinstall: pd-build
	-rm -rf $(LOCINSTALL_DIR)/*
	make -C pd/src install

pd-clean:
	make -C pd/src clean

pd: pd-locinstall

######## doit #################################
build: pd-build

locinstall: pd-locinstall externals

clean: pd-clean
	make -C libs clean

distclean: clean
	rm -rf $(LOCINSTALL_DIR)/*

########### external libs ####################
externals:
	make -C libs PDSOURCE=$(PWD)/pd/src
