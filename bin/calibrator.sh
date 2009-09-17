#!/bin/sh

#ECHO=echo

CUBEMIXERPATH=$(dirname $0)/..

# source functions
. ${CUBEMIXERPATH}/lib/functions.sh

cd ${CUBEMIXERPATH}/tools/calibrator/ || (echo "directory for CALIBRATOR not found!!"; exit -1)

./calibrate.sh
