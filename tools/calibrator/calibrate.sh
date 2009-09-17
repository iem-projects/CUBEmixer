#!/bin/sh

cd ${0%/*}

. ../../lib/functions.sh

CUBEMIXERPATH=$(dirname $0)/../..

CALIBRATOR_PATCH="CALIBRATOR.pd"
CALIBRATOR_PATH="-path ${CUBEMIXERPATH}/lib/libs/"
CALIBRATOR_LIBS="-lib zexy:iemmatrix"


PDINSTANCE=${CUBEMIXERPATH}/lib/pd/bin/pd



LOCALVARSTEMPLATE="${CUBEMIXERPATH}/etc/CUBEmixer/LocalVars.template.sh"
LOCALVARS="${CUBEMIXERPATH}/etc/CUBEmixer/LocalVars.sh"

if [ -e ${LOCALVARSTEMPLATE} ]; then
 . ${LOCALVARSTEMPLATE}
fi
if [ -e ${LOCALVARS} ]; then
 . ${LOCALVARS}
fi

PD=$(test_pd  ${PDINSTANCE} ${PD})

$ECHO $PD ${DSP_OPTIONS} ${DSP_AUDIO}  ${CALIBRATOR_PATH} ${CALIBRATOR_LIBS} ${CALIBRATOR_PATCH}
