# CUBEmixer: for the IEM CUBE and other environments
# Institut of Electronic Music and Acoustics Graz - 2005
# Programming-Concept and Development: Musil, Ritsch, Zmoelnig
# based on  various developments on IEM Graz 
#
# COMPILE / INSTALL INSTRUCTIONS for OS-X by 
# (02/20009) Noisternig / Zmoelnig
#============================================================= */


HOW-TO COMPILE / INSTALL the CUBEMIXER EXTERNAL LIBRARIES on OS-X
------------------------------------------------------------------

To run the CUBEmixer application on OS-X, you'll have to compile the following external libraries. The source codes are located in 
	CUBEmixer/src/libs/

Before compiling you have to check-out the current release of the CUBEmixer applications and all libraries. Please see INSTALL.txt for further information. You can download the latest stable package from the IEM project page 
	http://ambisonics.iem.at/xchange/products/cubemixer/
or check-out the current developer's version from SVN
	http://svn.sourceforge.net/viewcvs.cgi/iem/spatialization/CUBEmixer/branches/

The following section provides brief instructions on how to compile the external libraries on OS-X using the shell, which are ...
	- zexy
	- iemlib
	- iem_ambi, iem_ambi2, iem_bin_ambi
	- iemgui
	- iemmatrix
	- hcs
	- osc
	- net
	- popup
	- mixed

Please read the following compile instructions carefully. As soon as you have built a library, copy all *.d_fat and *.pd files to
	CUBEmixer/lib/libs

Compile instructions:

zexy:
	% cd CUBEmixer/src/libs/zexy/src
	% aclocal && autoconf
	% autoconf
	% ./configure --with-pd=/Applications/Pd.app/Contents/Resources
	% make
	% cp *.pd_darwin ../abs/*.pd ../../../../lib/libs/	

iemlib:
	% cd CUBEmixer/src/libs/iemlib
	
	The iemlib folder contains several subfolders 
		./iem_mp3, ./iem_t3_lib, ./iemlib1, ./iemlib2
	For compiling you have to navigate to src directories of the 
	respective subfolders, e.g.
	% cd iem_mp3
	and compile
	% make -f makefile_d_fat
	
	To copy all binaries navigate to
		CUBEmixer/src/libs/iemlib
	and run
	% cp */*.pd */*.d_fat ../../../lib/libs/
	
iem_ambi, iem_ambi2, iem_bin_ambi, iemgui:
	Navigate to src directories of the respective subfolders, e.g
	% cd CUBEmixer/src/libs/iem_ambi/src
	% make -f makefile_d_fat
	% cd ..
	% cp *.pd *.d_fat ../../../lib/libs/

iemmatrix:
	% cd CUBEmixer/src/libs/iemmatrix/src
	% aclocal && autoconf
	% autoconf
	% ./configure --with-pd=/Applications/Pd.app/Contents/Resources
	% make
	% cp *.pd_darwin ../abs/*.pd ../doc/*.pd ../../../../lib/libs/
	
	NOTE:
	Building *._fat binaries needs to manually edit the Make.config
	file after running after the autoconf process
	Open Make.config in your prefered editor and edit the modify the 
	lines for LIBRARY_CFLAGS and LIBS, which should look like
		LIBRARY_CFLAGS = $(IFLAGS) $(DEFS) $(BUILDLIBRARY) -DPD $(WFLAGS) -g -O2 -fPIC $(CFLAGS) -arch i386 -arch ppc
		LIBS = -L/Applications/Pd.app/Contents/Resources/bin -lm -lc -arch i386 -arch ppc
	That means adding "-arch i386 -arch ppc" and deleting "-mms-bitfields"
	
	% ./makesource.sh
	% make
	% cp *.pd_darwin ../abs/*.pd ../doc/*.pd ../../../../lib/libs/
	
hcs
	% cd CUBEmixer/src/libs
	% cp hcs/folder_list.c folder_list/
	% PDSOURCE=/Applications/Pd.app/Contents/Resources/src/ make -C folder_list -f ../Makefile.subdir d_fat
	
osc	
	% cd CUBEmixer/src/libs
	% PDSOURCE=/Applications/Pd.app/Contents/Resources/src/ make -C osc -f ../Makefile.subdir d_fat
	% cp osc/*.d_fat osc/*.pd ../../lib/libs/
	
net
	% cd CUBEmixer/src/libs
	% PDSOURCE=/Applications/Pd.app/Contents/Resources/src/ make -C net  -f ../Makefile.subdir d_fat
	% cp net/*.d_fat net/*.pd ../../lib/libs/

popup
	% cd CUBEmixer/src/libs
	% PDSOURCE=/Applications/Pd.app/Contents/Resources/src/ make -C popup  -f ../Makefile.subdir d_fat
	% cp popup/*.d_fat popup/*.pd ../../lib/libs/
	
mixed
	% cd CUBEmixer/src/libs/miXed
	Edit the file Makefile.common and change the following line
		PD_DIR = $(ROOT_DIR)/../../pd/src
		to
		PD_DIR ?= $(ROOT_DIR)/../../pd/src

	Then run from the shell
	% PD_DIR=/Applications/Pd.app/Contents/Resources/src make
	% cp bin/seq.pd_darwin bin/midiflush.pd_darwin bin/midiformat.pd_darwin bin/midiparse.pd_darwin bin/tot.pd_darwin ../../../lib/libs
	