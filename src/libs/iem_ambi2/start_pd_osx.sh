#!/bin/sh

PD_AUDIO="-r 44100 -audiobuf 80"
PD_MIDI="-nomidi"
PD_PATH="-path ../iemlib/iemabs:../iemmatrix/abs:../zexy/abs"
PD_LIB="-lib ../iemlib/iemlib1:../iemlib/iemlib2:../iemgui:../iemmatrix:../zexy:iem_ambi2"

/Applications/Pd.app/Contents/Resources/bin/pd ${PD_AUDIO} ${PD_MIDI} ${PD_PATH} ${PD_LIB}
