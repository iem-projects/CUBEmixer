# Makefile for building and locally installing pd in CUBEmixer (winfried)
# note: you need only to do this if you dont want to use a already installed pd

EXT?=pd_linux

LOCINSTALL_DIR=../lib/pd
PDCONFIG=-enable-alsa --enable-jack

# how to make absolute pathname out of it ?

.PHONY: install locinstall checkpddir 

install: checkpddir locinstall

pd-build: 
        cd pd/src
	autoconf
	./configure --prefix=$(LOCINSTALL_DIR) $(PDCONFIG)
	make 

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
