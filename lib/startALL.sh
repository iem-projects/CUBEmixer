#!/bin/sh
if [ "x$PD" = "x" ]
then
 PD=pd
 PD=/usr/local/src/pd-0.37-4.patched/bin/pd
fi
  

PDPATHS="-path .:abs/:libs/:plugins/:GUI/:DSP/:GUI/abs:DSP/abs:libs/iemabs:libs/zexyabs"
PDLIBS="-lib zexy:iemlib1:iemlib2:iemgui:iemmatrix:iem_ambi"

PDDSPFLAGS="-32bit -rt -channels 24 -blocksize 16 -audiobuf 150"
PDGUIFLAGS="-nosound -nomidi"

${PD} ${PDPATHS} ${PDLIBS} ${PDDSPFLAGS} DSP/test_DSP+NET+MIDI.pd &


${PD} ${PDPATHS} ${PDLIBS} ${PDGUIFLAGS} GUI/test16+OUT.pd
