#!/bin/sh

cd ${0%/*}

. ../../lib/functions.sh

CUBEMIXERPATH=$(dirname $0)/../..

CALIBRATOR_PATCH="CALIBRATOR.pd"
CALIBRATOR_PATH="-path ${CUBEMIXERPATH}/lib/libs/"
CALIBRATOR_LIBS="-lib zexy:iemmatrix"

ETC=etc/CUBEmixer
ETC_PATH="-path ${ETC}"

LOCALVARSTEMPLATE="${ETC}/LocalVars.template.sh"
STARTUPPATCHES="${ETC}/configured.sh"
LOCALVARS="${LOCALETC}/LocalVars.sh" 

PD_INSTALL="lib/pd/bin"

if [ -e ${LOCALVARSTEMPLATE} ]; then
 . ${LOCALVARSTEMPLATE}             
fi                                  
if [ -e ${STARTUPPATCHES} ]; then   
 . ${STARTUPPATCHES}                
fi                                  
if [ -e ${LOCALVARS} ]; then        
 . ${LOCALVARS}                     
fi

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

$ECHO $PD ${DSP_OPTIONS} ${DSP_AUDIO}  ${CALIBRATOR_PATH} ${CALIBRATOR_LIBS} \
 ${DSP_OPTIONS} ${LOCALETC_PATH} ${ETC_PATH} ${DSP_PATH} ${PD_PATH} ${PD_HELPPATH} ${DSP_LIB} ${PD_LIB}\
 ${CALIBRATOR_PATCH}
