3D-mixer: for the IEM CUBE and other environments
iem -2005
Concept and Development: Musil, Ritsch, Zmoelnig
Project Leader: Ritsch
=================================================

./lib	pure-data patches (and externals)
./doc	documentation
./src	source-code (for externals)
./tools	toolkits, helper-applications,...

directory-layout:
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
  ./lib/libs			- libraries (binaries)
  ./lib/tests			- sandbox for experimenting



