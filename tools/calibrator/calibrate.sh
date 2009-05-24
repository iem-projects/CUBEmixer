#!/bin/sh

cd ${0%/*}

CALIBRATOR_PATCH="CALIBRATOR.pd"
CALIBRATOR_PATH="-path ../../lib/libs/"
CALIBRATOR_LIBS="-lib zexy:iemmatrix"

. ../../lib/functions.sh

PDINSTANCE=../../lib/pd/bin/pd

PD=$(test_pd $PD $PDINSTANCE)

$PD -rt -inchannels 2 ${CALIBRATOR_PATH} ${CALIBRATOR_LIBS} ${CALIBRATOR_PATCH}
