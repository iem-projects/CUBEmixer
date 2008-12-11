#!/bin/sh

current_dir=${0%/*}

cd $current_dir

echo $current_dir

PD_INST="/Applications/Pd-0.40-2.app/Contents/Resources"
PD_AUDIO="-r 44100 -audiobuf 46 -channels 2"
PD_MIDI="-nomidi"
PD_PATH="-path ambi_abs:ambi_abs_multiple:${PD_INST}/extra/iemabs:${PD_INST}/extra/zexy/abs"
PD_LIB="-lib iemlib1:iemlib2:zexy:iem_ambi2:iem_matrix:iemmatrix:iemgui:net:osc"
PD_PATCH="universal_ambisonic_decoder_matrix_creater.pd"


echo starting pd ...
${PD_INST}/bin/pd ${PD_AUDIO} ${PD_MIDI} ${PD_OPTIONS} ${PD_PATH} ${PD_LIB}  ${PD_PATCH}
