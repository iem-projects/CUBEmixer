CUBEmixer: for the IEM CUBE and other environments
Institut of Electronic Music and Acoustics Graz - 2005
Programming-Concept and Development: Musil, Ritsch, Zmoelnig
based on  various developments on IEM Graz 
=================================================
------------------------------------------------------------------------------------------------------
Please start Mixer with skript located in the 'bin' folder and optional edit for system requirements:

 linux: Cubemixer.sh
 windows: first start 'CUBEmixerDSP.bat' and then 'CubemixerGUI.bat'

./lib	pure-data patches (and externals)
./doc	documentation
./src	source-code (for externals and pd)
./tools	toolkits, helper-applications,...
------------------------------------------------------------------------------------------------------
directory-layout for patch:

 core modules:
  ./lib/CUE			- cue-triggering machine
  ./lib/DSP			- the audio-engine
  ./lib/GUI			- the Graphical User Interface
  ./lib/GUI/<name>/		- helper abstractions for the ./GUI/<name>.pd abs
  ./lib/MIDI			- interface to MIDI-hw/sw
  ./lib/MIDI/faderbox		- interface to MIDI-hw/faderboxes [THINK about the name]
  ./lib/MIDI/extern		- interface to external MIDI-sequencer
  ./lib/SEQUENCER		- cue-sequencer

 add-on modules: ???
  ./lib/plugins/<pluginname>/	- DSP/GUI/MIDI/... part of plugins
 helpers:
  ./lib/abs			- general abstractions
  ./lib/libs			- external libraries collector (binaries, if needed and not in system)
  ./lib/tests			- sandbox for experimenting
