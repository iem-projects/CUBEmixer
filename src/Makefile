# Makefile for building and locally installing pd in CUBEmixer (winfried)
# note: you need only to do this if you dont want to use a already installed pd

EXT?=pd_linux

LOCINSTALL_DIR=../lib/pd
# how to make absolute pathname out of it ?

PDCONFIG=-enable-alsa --enable-jack


.PHONY: install locinstall

install: locinstall

pd-build: 
	cd pd/src; autoconf; ./configure --prefix=$(LOCINSTALL_DIR) $(PDCONFIG)
	make -C pd/src

pd-locinstall: pd-build
	make -C pd/src install

pd-clean:
	make -C pd/src clean

pd: pd-locinstall

######## doit #################################

sources: folder_list-src

build: pd-build

locinstall: pd-locinstall

clean: pd-clean

distclean: clean
	rm -rf $(LOCINSTALL_DIR)/*
