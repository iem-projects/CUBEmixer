#!/bin/sh

# start ambidecalc patch from this directory
# TODO to start from the base directory to store matrix to the right place

cd  ${0%%/*}

# source functions
. ../../lib/functions.sh

PDINST=../../lib/pd/bin
PD=$(test_pd $PD ${PDINST}/pd)

pd -path ../../lib/libs:../../lib/DSP/abs -lib iemmatrix:zexy:iem_ambi2:iemgui:iemlib1:iemlib2 ambidecigner.pd
