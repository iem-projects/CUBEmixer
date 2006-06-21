EXT=pd_linux

CVSROOT=-d:pserver:anonymous@cvs.pd.iem.at:/cvsroot/pure-data
REVISION=HEAD

LOCINSTALL_DIR=../../lib/libs/

.PHONY: install locinstall checkpddir aconnect-src iem_ambi-src iem_bin_ambi-src iemgui-src iemlib-src iemmatrix-src popup-src zexy-src 

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

######## AMIXER ################
#amixer-src: sf-login
#	cvs $(CVSROOT) checkout -r $(REVISION) -d amixer externals/iem/amixer/
#amixer-build: amixer-src
#	make -C amixer
#amixer-locinstall: amixer-build
#	cp mixer/*.$(EXT) amixer/amixer.pd $(LOCINSTALL_DIR)
#amixer: amixer-locinstall

######## HDSPMMIXER ################
hdspm_mixer-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d hdspm_mixer externals/iem/hdspm_mixer/
hdspm_mixer-build: hdspm_mixer-src
	make -C hdspm_mixer
hdspm_mixer-locinstall: hdspm_mixer-build
	cp hdspm_mixer/*.$(EXT) hdspm_mixer/hdspmmixer-help.pd $(LOCINSTALL_DIR)
hdspm_mixer: hdspm_mixer-locinstall


######## ACONNECT ################

aconnect-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d aconnect externals/iem/aconnect/

aconnect-build: aconnect-src
	make -C aconnect

aconnect-locinstall: aconnect-build
	cp aconnect/*.$(EXT) aconnect/aconnects.pd $(LOCINSTALL_DIR)

aconnect: aconnect-locinstall

######### IEM AMBI ###############

iem_ambi-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d iem_ambi externals/iem/iem_ambi/

iem_ambi-build: iem_ambi-src
	make -C iem_ambi/src

iem_ambi-locinstall: iem_ambi-build
	cp iem_ambi/src/iem_ambi.$(EXT) $(LOCINSTALL_DIR)

iem_ambi: iem_ambi-locinstall

######### IEM BIN AMBI ###############

iem_bin_ambi-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d iem_bin_ambi externals/iem/iem_bin_ambi/

iem_bin_ambi-build: iem_bin_ambi-src
	make -C iem_bin_ambi/src

iem_bin_ambi-locinstall: iem_bin_ambi-build
	cp iem_bin_ambi/src/iem_bin_ambi.$(EXT) $(LOCINSTALL_DIR)

iem_bin_ambi: iem_bin_ambi-locinstall

######## IEM GUI ##################

iemgui-src: 

iemgui-build: iemgui-src
	make -C ../iemgui 

iemgui-locinstall: iemgui-build
	cp ../iemgui/iemgui.$(EXT) $(LOCINSTALL_DIR)

iemgui: iemgui-locinstall

######## IEM SPEC2 ##################

iem_spec2-src: 

iem_spec2-build: iem_spec2-src
	make -C ../iem_spec2 

iem_spec2-locinstall: iem_spec2-build
	cp ../iem_spec2/iem_spec2.$(EXT) $(LOCINSTALL_DIR)

iem_spec2: iem_spec2-locinstall

######## IEM LIB ##################

iemlib-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d iemlib externals/iemlib/
	mkdir -p iemlib/lib

iemlib-build: iemlib-src
	make -C iemlib

iemlib-locinstall: iemlib-build
	cp iemlib/lib/*.$(EXT) $(LOCINSTALL_DIR)
	cp iemlib/iemabs/*.pd $(LOCINSTALL_DIR)

iemlib: iemlib-locinstall

####### IEM Matrix ###############

iemmatrix-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d iemmatrix externals/iem/iemmatrix/

iemmatrix-build: iemmatrix-src
	cd iemmatrix/src && autoconf && ./configure
	make -C iemmatrix/src

iemmatrix-locinstall: iemmatrix-build
	cp iemmatrix/*.$(EXT) $(LOCINSTALL_DIR)
	cp iemmatrix/abs/*.pd $(LOCINSTALL_DIR)

iemmatrix: iemmatrix-locinstall


########### miXed needed for the seq external in cyclone ##########################
# maybe substituted by xeq in future

miXed-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d miXed externals/miXed/

miXed-patched:
	-patch -N miXed/Makefile.common < miXed_makefile.common.diff

miXed-build: miXed-src miXed-patched 
	make -C miXed 

mixed_objects=seq midiflush midiformat midiparse
mixed_externals=$(mixed_objects:=.pd_linux)


miXed-locinstall: miXed-build
	for i in $(mixed_externals); do cp -v miXed/bin/$$i $(LOCINSTALL_DIR); done
	for i in $(mixed_objects); do cp -v miXed/test/cyclone/$$i-test.pd $(LOCINSTALL_DIR)/$$i-help.pd; done

######## IEM GUI ##################
# maybe substituted by iem-gui in future

popup-src_: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d popup externals/bbogart/popup/

popup-build_: popup-src
	make -C popup pd_linux

popup-locinstall_: popup-build_
	cp popup/*.$(EXT) $(LOCINSTALL_DIR)

# ############  local popup ###############
popup-src: 

popup-build: popup-src
	make -C ../popup pd_linux

popup-locinstall: popup-build
	cp ../popup/*.$(EXT) $(LOCINSTALL_DIR)

popup: popup-locinstall


######## ZEXY ##################

zexy-src: sf-login
	cvs $(CVSROOT) checkout -r $(REVISION) -d zexy externals/zexy 

zexy-build: zexy-src
	make -C zexy/src

zexy-locinstall: zexy-build
	cp zexy/*.$(EXT) $(LOCINSTALL_DIR)
	cp zexy/abs/*.pd $(LOCINSTALL_DIR)

zexy: zexy-locinstall

######## doit #################################

sources: aconnect-src iem_ambi-src iemgui-src iemlib-src iemmatrix-src popup-src zexy-src

build: aconnect-build iem_ambi-build iemgui-build iemlib-build iemmatrix-build popup-build zexy-build

locinstall: aconnect-locinstall iem_ambi-locinstall iem_bin_ambi-locinstall iemgui-locinstall iemlib-locinstall iemmatrix-locinstall popup-locinstall zexy-locinstall iem_ambi-locinstall iem_spec2-locinstall

clean: 
	rm -rf aconnect iem_ambi iemlib iemmatrix popup zexy sf-login

distclean: clean
	rm -rf $(LOCINSTALL_DIR)/*
