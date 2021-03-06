EXT?=pd_linux

CVSROOT=-d:pserver:anonymous@cvs.pd.iem.at:/cvsroot/pure-data
REVISION=HEAD

LOCINSTALL_DIR=../../lib/libs/
# since libs/* is cleaned we need it outside
LOCHELP_DIR=$(LOCINSTALL_DIR)/../helps/

PDSOURCE?=$(CURDIR)/../pd/src

.PHONY: install locinstall checkpddir 

install: checkpddir locinstall

ifeq (,$(findstring clean, $(MAKECMDGOALS)))
checkpddir:
	@if [ ! -d "$(PDSOURCE)" ]; then \
	  echo "please give the path to your PD-Sources with: make PDSOURCE=/path/to/pd/src";\
          echo "for local installed PD use as  path an absolute path to the PDSOURCE=pathto_CUBEMIXER/src/pd/src";\
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
hdspm_mixer-build: 
	make -C hdspm_mixer

hdspm_mixer-locinstall: hdspm_mixer-build
	cp hdspm_mixer/*.$(EXT) $(LOCINSTALL_DIR)
	cp hdspm_mixer/hdspmmixer-help.pd $(LOCHELP_DIR)

hdspm_mixer: hdspm_mixer-locinstall

hdspm_mixer-clean:



######## ACONNECT ################

aconnect-build:
	make -C aconnect INLCUDES="-I$(PDSOURCE)"

aconnect-locinstall: aconnect-build
	cp aconnect/*.$(EXT) aconnect/aconnects.pd $(LOCINSTALL_DIR)
	cp aconnect/*-help.pd $(LOCHELP_DIR)

aconnect: aconnect-locinstall

aconnect-clean:
	make -C aconnect clean

######### hcs's folder_list ###############

folder_list/folder_list.c: hcs/folder_list.c
	cp hcs/folder_list.c folder_list/folder_list.c

folder_list-build: folder_list/folder_list.c
	PDSOURCE=$(PDSOURCE) make -C folder_list -f ../Makefile.subdir
#	$(CC) -DPD -O2 -funroll-loops -fomit-frame-pointer -fPIC -I$(PDSOURCE) -export_dynamic -shared -lc -lm -o folder_list/folder_list.$(EXT) folder_list/folder_list.c

folder_list-locinstall: folder_list-build
	cp folder_list/folder_list.$(EXT) $(LOCINSTALL_DIR)

folder_list: folder_list-locinstall

folder_list-clean:
	make -C folder_list -f ../Makefile.subdir clean

######### IEM AMBI ###############

iem_ambi-build: 
	make -C iem_ambi/src PDSOURCE=$(PDSOURCE)

iem_ambi-locinstall: iem_ambi-build
	cp iem_ambi/iem_ambi.$(EXT) $(LOCINSTALL_DIR)
	cp iem_ambi/*-help.pd $(LOCHELP_DIR)

iem_ambi: iem_ambi-locinstall

iem_ambi-clean:
	make -C iem_ambi/src clean

######### IEM AMBI2 ###############

iem_ambi2-build: 
	make -C iem_ambi2/src -f makefile_linux

iem_ambi2-locinstall: iem_ambi2-build
	cp iem_ambi2/src/iem_ambi2.$(EXT) $(LOCINSTALL_DIR)
	cp iem_ambi2/*-help.pd $(LOCHELP_DIR)

iem_ambi2: iem_ambi2-locinstall

iem_ambi2-clean:
	make -C iem_ambi2/src -f makefile_linux clean


######### IEM BIN AMBI ###############

iem_bin_ambi-build:
	make -C iem_bin_ambi/src PDSOURCE=$(PDSOURCE)

iem_bin_ambi-locinstall: iem_bin_ambi-build
	cp iem_bin_ambi/src/iem_bin_ambi.$(EXT) $(LOCINSTALL_DIR)
	cp iem_bin_ambi/*-help.pd $(LOCHELP_DIR)

iem_bin_ambi: iem_bin_ambi-locinstall

iem_bin_ambi-clean:

######## IEM GUI ##################

iemgui-build: 
	make -C iemgui/src PDSOURCE=$(PDSOURCE)

iemgui-locinstall: iemgui-build
	cp iemgui/iemgui.$(EXT) $(LOCINSTALL_DIR)
	cp iemgui/*-help.pd $(LOCHELP_DIR)


iemgui: iemgui-locinstall

iemgui-clean:

######## IEM SPEC2 ##################

iem_spec2-build:
	make -C iem_spec2/src

iem_spec2-locinstall: iem_spec2-build
	cp iem_spec2/iem_spec2.$(EXT) $(LOCINSTALL_DIR)
	cp iem_spec2/*-help.pd $(LOCHELP_DIR)

iem_spec2: iem_spec2-locinstall

iem_spec2-clean:

######## IEM LIB ##################

iemlib-build:
	make -C iemlib PDSOURCE=$(PDSOURCE)

iemlib-locinstall: iemlib-build
	cp iemlib/lib/*.$(EXT) $(LOCINSTALL_DIR)
	cp iemlib/iemabs/*.pd $(LOCINSTALL_DIR)
	cp iemlib/iemlib1/*-help.pd $(LOCHELP_DIR)
	cp iemlib/iemlib2/*-help.pd $(LOCHELP_DIR)
	cp iemlib/iem_mp3/*-help.pd $(LOCHELP_DIR)
	cp iemlib/iem_t3_lib/*-help.pd $(LOCHELP_DIR)


iemlib: iemlib-locinstall

iemlib-clean:

####### IEM Matrix ###############

iemmatrix-build:
	make -C iemmatrix/src

iemmatrix-locinstall: iemmatrix-build
	cp iemmatrix/*.$(EXT) $(LOCINSTALL_DIR)
	cp iemmatrix/abs/*.pd $(LOCINSTALL_DIR)
	cp iemmatrix/doc/*-help.pd $(LOCHELP_DIR)

iemmatrix: iemmatrix-locinstall

iemmatrix-clean:

########### miXed needed for the seq external in cyclone ##########################
# maybe substituted by xeq in future

miXed-build:
	make -C miXed PD_DIR=$(PDSOURCE)

miXed_objects=seq midiflush midiformat midiparse tot
miXed_externals=$(miXed_objects:=.$(EXT))
miXed_helps=$(miXed_objects:=-help.pd)


miXed-locinstall: miXed-build
	for i in $(miXed_externals); do cp -v miXed/bin/$$i $(LOCINSTALL_DIR); done
#	for i in $(miXed_objects); do cp -v miXed/test/cyclone/$$i-test.pd $(LOCINSTALL_DIR)/$$i-help.pd; done
	for i in $(miXed_externals); do cp -v miXed/bin/$$i $(LOCINSTALL_DIR); done
	for i in $(miXed_helps); do find miXed/ -name $$i -exec cp -v \{} $(LOCHELP_DIR) \; ; done

miXed: miXed-locinstall

miXed-clean:

########### pdogg needed for streaming  ##########################

pdogg-build:
	make -C pdogg LINUXINCLUDE="-I $(PDSOURCE)" $(EXT)

pdogg_objects=oggamp~  oggcast~  oggread~  oggwrite~
pdogg_externals=$(pdogg_objects:=.$(EXT))
pdogg_helps=$(pdogg_objects:=-help.pd)

pdogg-locinstall: pdogg-build
	cp -v pdogg/pdogg.$(EXT) $(LOCINSTALL_DIR)
	for i in $(pdogg_objects); do cp -v pdogg/$$i/*help.pd $(LOCHELP_DIR); done

pdogg: pdogg-locinstall

pdogg-clean:
	make -C pdogg clean

########### pdogg needed for streaming  ##########################

Gem-build:
	make -C Gem/src LINUXINCLUDE="-I $(PDSOURCE)" 

Gem_objects=Gem
Gem_externals=$(Gem_objects:=.$(EXT))

Gem-locinstall: Gem-build
	cp -v Gem/src/Gem.$(EXT) $(LOCINSTALL_DIR)
	cp -v Gem/help/* $(LOCHELP_DIR)

Gem: Gem-locinstall

Gem-clean:
	make -C Gem/src clean

# ############  local popup ###############
popup-build:
	make -C popup $(EXT) PDSOURCE=$(PDSOURCE)

popup-locinstall: popup-build
	cp popup/*.$(EXT) $(LOCINSTALL_DIR)
	cp popup/*-help.pd $(LOCHELP_DIR)

popup: popup-locinstall

popup-clean:
	make -C popup clean

######## OSC ##################

osc-build: 
	make -C osc -f ../Makefile.subdir $(EXT)

osc-locinstall: osc-build
	cp osc/*.$(EXT) $(LOCINSTALL_DIR)
	cp osc/*-help.pd $(LOCHELP_DIR)

osc-clean:

osc: osc-locinstall

######## NET ##################

net-build:
	make -C net -f ../Makefile.subdir $(EXT) PDSOURCE=$(PDSOURCE)

net-locinstall: net-build
	cp net/*.$(EXT) $(LOCINSTALL_DIR)
	cp net/*-help.pd $(LOCHELP_DIR)

net-clean:

net: net-locinstall

######## ZEXY ##################

zexy-build: 
	make -C zexy/src CONFIGUREFLAGS="--includedir=$(PDSOURCE)"

zexy-locinstall: zexy-build
	cp zexy/*.$(EXT) $(LOCINSTALL_DIR)
	cp zexy/abs/*.pd $(LOCINSTALL_DIR)
	cp zexy/reference/*-help.pd $(LOCHELP_DIR)

zexy-clean:

zexy: zexy-locinstall

######## doit #################################

sources: folder_list-src

build: aconnect-build folder_list-build iem_ambi-build iem_ambi2-build iem_bin_ambi-build iemgui-build iemlib-build iemmatrix-build iem_spec2-build miXed-build net-build osc-build popup-build zexy-build

locinstall: aconnect-locinstall folder_list-locinstall iem_ambi-locinstall iem_ambi2-locinstall iem_bin_ambi-locinstall iemgui-locinstall iemlib-locinstall iemmatrix-locinstall iem_spec2-locinstall miXed-locinstall net-locinstall osc-locinstall popup-locinstall zexy-locinstall
	mv $(LOCINSTALL_DIR)/*-help.pd $(LOCHELP_DIR)/

optional: pdogg

clean: aconnect-clean folder_list-clean iem_ambi-clean iem_ambi2-clean iem_bin_ambi-clean iemgui-clean iemlib-clean iemmatrix-clean iem_spec2-clean miXed-clean net-clean osc-clean popup-clean zexy-clean pdogg-clean
	rm -f sf-login

distclean: clean
	rm -rf $(LOCINSTALL_DIR)/*
	rm -rf $(LOCHELP_DIR)/*

