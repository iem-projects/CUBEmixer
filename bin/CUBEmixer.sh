#!/bin/sh



## this function returns the first of its arguments that is
## an executable file
## if no executable files are given, it returns the default
## pd-binary (if any)
## usage:
##  test_pd /path/to/my/preferred/pd /path/to/my/second/choice/pd
function test_pd() {
local i
local ARGS
ARGS="$@ $(which pd)"
for i in $ARGS
do
  if [ -x "$i" ]
  then
    echo $i
    return
  fi
done
}

CUBEMIXERPATH=$(dirname $0)/..
cd ${CUBEMIXERPATH}

PD=$(test_pd src/pd/bin/pd $PD)

if [ "x$PD" = "x" ]
then
  echo "pd not found!"
  echo "set the environment variable PD to the full-path to the pd binary"
  echo "e.g.: \"export PD=/usr/bin/pd\""
  echo
  exit -1
fi
  

PDPATHS="-path .:lib:lib/abs/:lib/libs/:lib/plugins/:lib/extensions/:lib/GUI/:lib/DSP/:lib/GUI/abs:lib/DSP/abs:lib/libs/iemabs:lib/libs/zexyabs"
PDLIBS="-lib zexy:iemlib1:iemlib2:iemgui:iemmatrix:iem_ambi"

#PDDSPFLAGS="-32bit -rt -channels 24 -blocksize 16 -audiobuf 50 -alsamidi -mididev 1"
PDDSPFLAGS="-alsa -rt -channels 26 -r 44100 -audiobuf 23 -alsamidi -mididev 1"

PDGUIFLAGS="-nosound -nomidi -nrt"

${PD} ${PDPATHS} ${PDLIBS} ${PDDSPFLAGS} -open lib/DSP/DSP+NET+MIDI+CUE.pd ${CUBEMIXER_DSP_FLAGS} &


${PD} ${PDPATHS} ${PDLIBS} ${PDGUIFLAGS} -open lib/GUI/GUI16+OUT.pd  ${CUBEMIXER_GUI_FLAGS}

