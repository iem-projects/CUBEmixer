EXT=pd_linux

CVSROOT=-d:pserver:anonymous@cvs.pd.iem.at:/cvsroot/pure-data
REVISION=HEAD

LOCINSTALL_DIR=../../lib/libs/

.PHONY: install locinstall checkpddir aconnect-src iem_ambi-src iemgui-src iemlib-src iemmatrix-src popup-src zexy-src

install: checkpddir locinstall

ifeq (,$(findstring clean, $(MAKECMDGOALS)))
checkpddir:
	@if [ ! -d "$(PDSOURCE)" ]; then \
	  echo "please give the path to your PD-Sources with: make PDSOURCE=/path/to/pd/src";\
	exit -1; fi
endif

sf-login:
	cvs $(CVSROOT) login
	touch sf-login

aconnect-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d aconnect externals/iem/aconnect/

aconnect-build: aconnect-src
	make -C aconnect

aconnect-locinstall: aconnect-build
	cp aconnect/*.$(EXT) aconnect/aconnects.pd $(LOCINSTALL_DIR)

aconnect: aconnect-locinstall

iem_ambi-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d iem_ambi externals/iem/iem_ambi/

iem_ambi-build: iem_ambi-src
	make -C iem_ambi/src

iem_ambi-locinstall: iem_ambi-build
	cp iem_ambi/src/iem_ambi.$(EXT) $(LOCINSTALL_DIR)

iem_ambi: iem_ambi-locinstall

iemgui-src: 

iemgui-build: iemgui-src
	make -C ../iemgui 

iemgui-locinstall: iemgui-build
	cp ../iemgui/iemgui.$(EXT) $(LOCINSTALL_DIR)

iemgui: iemgui-locinstall

iemlib-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d iemlib externals/iemlib/
	mkdir -p iemlib/lib

iemlib-build: iemlib-src
	make -C iemlib

iemlib-locinstall: iemlib-build
	cp iemlib/lib/*.$(EXT) $(LOCINSTALL_DIR)
	cp iemlib/iemabs/*.pd $(LOCINSTALL_DIR)

iemlib: iemlib-locinstall

iemmatrix-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d iemmatrix externals/iem/iemmatrix/

iemmatrix-build: iemmatrix-src
	cd iemmatrix/src && autoconf && ./configure
	make -C iemmatrix/src

iemmatrix-locinstall: iemmatrix-build
	cp iemmatrix/*.$(EXT) $(LOCINSTALL_DIR)
	cp iemmatrix/abs/*.pd $(LOCINSTALL_DIR)

iemmatrix: iemmatrix-locinstall

popup-src_: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d popup externals/bbogart/popup/

popup-build_: popup-src
	make -C popup pd_linux

popup-locinstall_: popup-build_
	cp popup/*.$(EXT) $(LOCINSTALL_DIR)

popup-src: 

popup-build: popup-src
	make -C ../popup pd_linux

popup-locinstall: popup-build
	cp ../popup/*.$(EXT) $(LOCINSTALL_DIR)

popup: popup-locinstall

zexy-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d zexy externals/zexy 

zexy-build: zexy-src
	make -C zexy/src

zexy-locinstall: zexy-build
	cp zexy/*.$(EXT) $(LOCINSTALL_DIR)
	cp zexy/abs/*.pd $(LOCINSTALL_DIR)

zexy: zexy-locinstall

sources: aconnect-src iem_ambi-src iemgui-src iemlib-src iemmatrix-src popup-src zexy-src

build: aconnect-build iem_ambi-build iemgui-build iemlib-build iemmatrix-build popup-build zexy-build

locinstall: aconnect-locinstall iem_ambi-locinstall iemgui-locinstall iemlib-locinstall iemmatrix-locinstall popup-locinstall zexy-locinstall

clean: 
	rm -rf aconnect iem_ambi iemlib iemmatrix popup zexy sf-login

distclean: clean
	rm -rf $(LOCINSTALL_DIR)/*
