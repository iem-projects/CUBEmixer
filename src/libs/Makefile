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

aconnect-locinstall:
	cp aconnect/*.$(EXT) aconnect/aconnects.pd $(LOCINSTALL_DIR)

iem_ambi-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d iem_ambi externals/iem/iem_ambi/

iem_ambi-build: iem_ambi-src
	make -C iem_ambi

iem_ambi-locinstall:

iemgui-src: 

iemgui-build: iemgui-src
	make -C ../iemgui 

iemgui-locinstall:
	cp ../iemgui/iemgui.$(EXT) $(LOCINSTALL_DIR)

iemlib-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d iemlib externals/iemlib/
	mkdir -p iemlib/lib

iemlib-build: iemlib-src
	make -C iemlib

iemlib-locinstall: 
	cp iemlib/lib/*.$(EXT) $(LOCINSTALL_DIR)
	cp iemlib/iemabs/*.pd $(LOCINSTALL_DIR)

iemmatrix-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d iemmatrix externals/iem/iemmatrix/

iemmatrix-build: iemmatrix-src
	cd iemmatrix/src && autoconf && ./configure
	make -C iemmatrix/src

iemmatrix-locinstall: 
	cp iemmatrix/*.$(EXT) $(LOCINSTALL_DIR)
	cp iemmatrix/abs/*.pd $(LOCINSTALL_DIR)

popup-src_: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d popup externals/bbogart/popup/

popup-build_: popup-src
	make -C popup pd_linux

popup-locinstall_: 
	cp popup/*.$(EXT) $(LOCINSTALL_DIR)

popup-src: 

popup-build: popup-src
	make -C ../popup pd_linux

popup-locinstall: 
	cp ../popup/*.$(EXT) $(LOCINSTALL_DIR)


zexy-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d zexy externals/zexy 

zexy-build: zexy-src
	make -C zexy/src

zexy-locinstall: 
	cp zexy/*.$(EXT) $(LOCINSTALL_DIR)
	cp zexy/abs/*.pd $(LOCINSTALL_DIR)


sources: aconnect-src iemgui-src iemlib-src iemmatrix-src popup-src zexy-src

build: sources aconnect-build iemgui-build iemlib-build iemmatrix-build popup-build zexy-build

locinstall: build aconnect-locinstall iemgui-locinstall iemlib-locinstall iemmatrix-locinstall popup-locinstall zexy-locinstall

clean: 
	rm -rf aconnect iem_ambi iemlib iemmatrix popup zexy sf-login

distclean: clean
	rm -rf $(LOCINSTALL_DIR)/*
