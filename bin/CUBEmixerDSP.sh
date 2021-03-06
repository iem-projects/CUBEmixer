#!/bin/sh

## usage:
##    /path/to/CUBEmixerDSP.sh [<extrapatch> [<extrapath>]]
## starts the DSP-engine of the cubemixer with optionally loading an additional "extrapatch"
## if you need additional paths for the "extrapatch", specify them as "extrapath"
## example:
##    /path/to/CUBEmixerDSP.sh my_concertDSP.pd /path/to/my_concert



#ECHO=echo
ETC=etc/CUBEmixer

CUBEMIXERPATH=$(dirname $0)/..
cd ${CUBEMIXERPATH}
. lib/functions.sh

LOCALVARSTEMPLATE="${ETC}/LocalVars.template.sh"
STARTUPPATCHES="${ETC}/configured.sh"
LOCALVARS="${ETC}/LocalVars.sh"

if [ -e ${LOCALVARSTEMPLATE} ]; then
 . ${LOCALVARSTEMPLATE}
fi
if [ -e ${STARTUPPATCHES} ]; then
 . ${STARTUPPATCHES}
fi
if [ -e ${LOCALVARS} ]; then
 . ${LOCALVARS}
fi

PD=$(test_pd $PD ${PD_INSTALL}/pd)

if [ "x$PD" = "x" ]
then
  echo "pd not found!"
  echo "set the environment variable PD to the full-path to the pd binary"
  echo "e.g.: \"export PD=/usr/bin/pd\""
  echo
  exit -1
fi

if [ "x" != "x${DSP_PATCH}" ]; then
 DSP_PATCH="-open ${DSP_PATCH}"
fi

if [ "x" != "x${1}" ]; then
 if [ -f $1 ]; then
    DSP_EXTPATCH="-open $1"
 fi
fi

if [ "x" != "x${2}" ]; then
    DSP_EXTPATH="-path $2"
fi

let DSP_ADCS=DSP_ADCS
if [ ${DSP_ADCS} -gt 0 ]; then
  DSP_AUDIO="${DSP_AUDIO} -inchannels ${DSP_ADCS}"
fi

let DSP_DACS=DSP_DACS
if [ ${DSP_DACS} -gt 0 ]; then
  DSP_AUDIO="${DSP_AUDIO} -outchannels ${DSP_DACS}"
fi


${ECHO} ${PD} ${DSP_AUDIO} ${DSP_MIDI} ${DSP_OPTIONS} ${PD_OPTIONS} ${DSP_PATH} ${PD_PATH} ${DSP_LIB} ${PD_LIB} ${DSP_PATCH}  ${DSP_EXTPATH} ${DSP_EXTPATCH} \
      -send "${DSP_MESSAGE}" -send "${PD_MESSAGE}"
