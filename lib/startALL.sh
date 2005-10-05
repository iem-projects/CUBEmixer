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

PD=$(test_pd $PD /usr/local/src/pd-0.37-4.patched/bin/pd)

if [ "x$PD" = "x" ]
then
  echo "pd not found!"
  echo "set the environment variable PD to the full-path to the pd binary"
  echo "e.g.: \"export PD=/usr/bin/pd\""
  echo
  exit -1
fi
  

PDPATHS="-path .:abs/:libs/:plugins/:GUI/:DSP/:GUI/abs:DSP/abs:libs/iemabs:libs/zexyabs"
PDLIBS="-lib zexy:iemlib1:iemlib2:iemgui:iemmatrix:iem_ambi"

PDDSPFLAGS="-32bit -rt -channels 24 -blocksize 16 -audiobuf 150 -alsamidi"
PDGUIFLAGS="-nosound -nomidi"

${PD} ${PDPATHS} ${PDLIBS} ${PDDSPFLAGS} DSP/DSP+NET+MIDI+CUE.pd &


${PD} ${PDPATHS} ${PDLIBS} ${PDGUIFLAGS} GUI/GUI16+OUT.pd
