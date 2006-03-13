CUBEmixer: for the IEM CUBE and other environments
Institut of Electronic Music and Acoustics Graz - 2005
Programming-Concept and Development: Musil, Ritsch, Zmoelnig
based on  various developments on IEM Graz 
=================================================
-------------------------------------------------------------------------------
Please start Mixer with skript located in the 'bin' folder and optional edit 
 for system requirements:

 linux: Cubemixer.sh
 windows: first start 'CUBEmixerDSP.bat' and then 'CubemixerGUI.bat'

./lib	pure-data patches (and externals)
./doc	documentation
./src	source-code (for externals and pd)
./tools	toolkits, helper-applications,...
./etc   configuration

-------------------------------------------------------------------------------
configuring the CUBEmixer to run on your system:
 all (local) configuration of the CUBEmixer should be done in the etc/CUBEmixer
 folder
 IMPORTANT: if you have pd installed in a non-standard directory, you have
 to specify the PD_INSTALL path in the configuration. READ ON !!

 the configuration is done via a template-system. for each OS, you will find
 a template file (e.g. LocalVars.template.bat); this file is both a 
 documentation (to show you, which variables can be set) and it also holds
 the default variables.
 To modify these variables proceed as follows:
  - COPY the LocalVars.template file to LocalVars - preserving the extension
     on windows: copy "LocalVars.template.bat" to "LocalVars.bat"
     on unix   : copy "LocalVars.template.sh" to "LocalVars.sh"
  - DO NOT delete LocalVars.template
  - then modify your LocalVars file, to reflect the settings of your system
  - you can ignore (delete) all lines (in your new file) where you don't 
    want to override the defaults.

 if you checked out the CUBEmixer via a source-distribution (e.g. Subversion),
 chances are high that you need to set PD_INSTALL in LocalVars.template.bat,
 e.g. with something like "set PD_INSTALL=C:\Programs\pd"

-------------------------------------------------------------------------------
directory-layout for patch:

 core modules:
  ./lib/CUE			- cue-triggering machine
  ./lib/DSP			- the audio-engine
  ./lib/GUI			- the Graphical User Interface
  ./lib/GUI/<name>/		- helper abstractions for the ./GUI/<name>.pd 
                                  abs
  ./lib/MIDI			- interface to MIDI-hw/sw
  ./lib/MIDI/faderbox		- interface to MIDI-hw/faderboxes [THINK about
                                  the name]
  ./lib/MIDI/extern		- interface to external MIDI-sequencer
  ./lib/SEQUENCER		- cue-sequencer

 add-on modules: ???
  ./lib/plugins/<pluginname>/	- DSP/GUI/MIDI/... part of plugins
 helpers:
  ./lib/abs			- general abstractions
  ./lib/libs			- external libraries collector (binaries, if 
                                  needed and not in system)
  ./lib/tests			- sandbox for experimenting

