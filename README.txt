3D-mixer: for the IEM CUBE and other environments
=================================================

./pd	pure-data patches (and externals)
./doc	documentation
./src	source-code (for externals)
./data	data

directory-layout:
 core modules:
  ./pd/CUE			- cue-triggering machine
  ./pd/DSP			- the audio-engine
  ./pd/GUI			- the Graphical User Interface
  ./pd/GUI/<name>/		- helper abstractions for the ./GUI/<name>.pd abs
  ./pd/MIDI			- interface to MIDI-hw/sw
  ./pd/MIDI/faderbox		- interface to MIDI-hw/faderboxes [THINK about the name]
  ./pd/MIDI/extern		- interface to external MIDI-sequencer
  ./pd/SEQUENCER		- cue-sequencer
 add-on modules: ???
  ./pd/plugins/<pluginname>/	- DSP/GUI/MIDI/... part of plugins
 helpers:
  ./pd/abs			- general abstractions
  ./pd/libs			- libraries (binaries)
  ./pd/tests			- sandbox for experimenting



