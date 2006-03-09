EXT=pd_linux

CVSROOT=-d:pserver:anonymous@cvs.pd.iem.at:/cvsroot/pure-data
LOCINSTALL_DIR=../lib/libs/

.PHONY: aconnect-src iem_ambi-src iemgui-src iemlib-src iemmatrix-src popup-src zexy-src

sf-login:
	cvs $(CVSROOT) login
	touch sf-login

aconnect-src: sf-login
	cvs $(CVSROOT) checkout -d aconnect externals/iem/aconnect/

aconnect-build: aconnect-src
	make -C aconnect

aconnect-locinstall:
	cp aconnect/*.$(EXT) aconnect/aconnects.pd $(LOCINSTALL_DIR)



iem_ambi-src: sf-login
	cvs $(CVSROOT) checkout -d iem_ambi externals/iem/iem_ambi/

iem_ambi-build: iem_ambi-src
	make -C iem_ambi

iem_ambi-locinstall:

iemgui-src: sf-login
	cvs $(CVSROOT) checkout -d iemgui externals/iem/iemgui/

iemgui-build: iemgui-src
	make -C iemgui

iemgui-locinstall:

iemlib-src: sf-login
	cvs $(CVSROOT) checkout -d iemlib externals/iemlib/

iemlib-build: iemlib-src
	make -C iemlib

iemlib-locinstall: 
	cp iemlib/lib/*.$(EXT) $(LOCINSTALL_DIR)
	cp iemlib/iemabs/*.pd $(LOCINSTALL_DIR)

iemmatrix-src: sf-login
	cvs $(CVSROOT) checkout -d iemmatrix externals/iem/iemmatrix/

iemmatrix-build: iemmatrix-src
	cd iemmatrix/src && autoconf && ./configure
	make -C iemmatrix/src

iemmatrix-locinstall: 
	cp iemmatrix/*.$(EXT) $(LOCINSTALL_DIR)
	cp iemmatrix/abs/*.pd $(LOCINSTALL_DIR)

popup-src: sf-login
	cvs $(CVSROOT) checkout -d popup externals/bbogart/popup/

popup-build: popup-src
	make -C popup pd_linux

popup-locinstall: 
	cp popup/*.$(EXT) $(LOCINSTALL_DIR)


zexy-src: sf-login
	cvs $(CVSROOT) checkout -d zexy externals/zexy 

zexy-build: zexy-src
	make -C zexy/src

zexy-locinstall: 
	cp zexy/*.$(EXT) $(LOCINSTALL_DIR)
	cp zexy/abs/*.pd $(LOCINSTALL_DIR)


sources: aconnect-src iemlib-src iemmatrix-src popup-src zexy-src

build: sources aconnect-build iemlib-build iemmatrix-build popup-build zexy-build

locinstall: build aconnect-locinstall iemlib-locinstall iemmatrix-locinstall popup-locinstall zexy-locinstall
