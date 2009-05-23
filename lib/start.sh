#!/bin/sh

cd `dirname $0`
. functions.sh

PD=$(test_pd $PD ${PDINSTALL}/pd)

PDPATHS="-path .:abs/:libs/:plugins/:GUI/:DSP/:GUI/abs:DSP/abs:libs/iemabs:libs/zexyabs"
PDLIBS="-lib zexy:iemlib1:iemlib2:iemgui:iemmatrix:iem_ambi"

${PD} $PDPATHS $PDLIBS $@
