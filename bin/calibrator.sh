#!/bin/sh

#ECHO=echo

CUBEMIXERPATH=$(dirname $0)/..

# source functions
. ${CUBEMIXERPATH}/lib/functions.sh

cd ${CUBEMIXERPATH}/tools/calibrator/ || (echo "directory for CALIBRATOR not found!!"; exit -1)

LOCALVARSTEMPLATE="etc/CUBEmixer/LocalVars.template.sh"
LOCALVARS="etc/CUBEmixer/LocalVars.sh"

if [ -e ${LOCALVARSTEMPLATE} ]; then
 . ${LOCALVARSTEMPLATE}
fi
if [ -e ${LOCALVARS} ]; then
 . ${LOCALVARS}
fi

PD=$(test_pd $PD ${PD_INSTALL}/pd)

./calibrate.sh
