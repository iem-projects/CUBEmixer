#!/bin/bash

# Start CUBEmixer with optional arguments

## ARGS: [DSP_PATCH] [GUI_PATCH] [DSP_PATH] [GUI_PATH]
##  dsp_patch ... external DSP-patch
##  dsp_patch ... external DSP Patch
##  dsp_path  ... external DSP path
##  dsp_patch ... external DSP path

#ECHO=echo
ETC=etc/CUBEmixer

CUBEMIXERPATH=$(dirname $0)/..
cd ${CUBEMIXERPATH}

# load helper functions
. lib/functions.sh

LOCALVARSTEMPLATE="${ETC}/LocalVars.template.sh"
STARTUPPATCHES="${ETC}/configured.sh"
LOCALVARS="${ETC}/LocalVars.sh"

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

PD=$(test_pd $PD ${PD_INSTALL}/pd)

if [ "x$PD" = "x" ]
then
  echo "pd not found!"
  echo "set the environment variable PD to the full-path to the pd binary"
  echo "e.g.: \"export PD=/usr/bin/pd\""
  echo
  exit -1
fi

if [ "x" != "x${GUI_PATCH}" ]; then
 GUI_PATCH="-open ${GUI_PATCH}"
fi

if [ "x" != "x${2}" ]; then
 if [ -f $2 ]; then
    GUI_EXTPATCH="-open $2"
 fi
fi


if [ "x" != "x${DSP_PATCH}" ]; then
 DSP_PATCH="-open ${DSP_PATCH}"
fi

if [ "x" != "x${1}" ]; then
 if [ -f $1 ]; then
    DSP_EXTPATCH="-open $1"
 fi
fi

if [ "x" != "x${3}" ]; then
    DSP_EXTPATH="-path $3"
fi

if [ "x" != "x${4}" ]; then
    GUI_EXTPATH="-path $4"
fi

let DSP_ADCS=DSP_ADCS
if [ ${DSP_ADCS} -gt 0 ]; then
  DSP_AUDIO="${DSP_AUDIO} -inchannels ${DSP_ADCS}"
fi

let DSP_DACS=DSP_DACS
if [ ${DSP_DACS} -gt 0 ]; then
  DSP_AUDIO="${DSP_AUDIO} -outchannels ${DSP_DACS}"
fi


if [ "x${NOGUI}" = "x" ]; then
${ECHO} ${PD} -noprefs ${GUI_AUDIO} ${GUI_MIDI} ${GUI_OPTIONS} ${PD_OPTIONS} ${ETC_PATH} ${GUI_EXTPATH} ${GUI_PATH} ${PD_PATH} ${PD_HELPPATH} ${GUI_LIB} ${PD_LIB} ${GUI_PATCH}  ${GUI_EXTPATCH} \
      -send "${GUI_MESSAGE}" -send "${PD_MESSAGE}" &
fi

if [ "x${NODSP}" = "x" ]; then
${ECHO} ${PD} -noprefs ${DSP_AUDIO} ${DSP_MIDI} ${DSP_OPTIONS} ${PD_OPTIONS} ${ETC_PATH} ${DSP_EXTPATH} ${DSP_PATH} ${PD_PATH} ${PD_HELPPATH} ${DSP_LIB} ${PD_LIB} ${DSP_PATCH}  ${DSP_EXTPATCH} \
      -send "${DSP_MESSAGE}" -send "${PD_MESSAGE}"
fi
