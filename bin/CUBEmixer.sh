#!/bin/sh



## this function returns the first of its arguments that is
## an executable file
## if no executable files are given, it returns the default
## pd-binary (if any)
## usage:
##  test_pd /path/to/my/preferred/pd /path/to/my/second/choice/pd
function test_pd() {
local i
local ARGS
ARGS="$@ $(which pd)"
for i in $ARGS
do
  if [ -x "$i" ]
  then
    echo $i
    return
  fi
done
}

CUBEMIXERPATH=$(dirname $0)/..
cd ${CUBEMIXERPATH}

LOCALVARSTEMPLATE="etc/CUBEmixer/LocalVars.template.sh"
LOCALVARS="etc/CUBEmixer/LocalVars.sh"

if [ -e ${LOCALVARSTEMPLATE} ]; then
 . ${LOCALVARSTEMPLATE}
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

if [ "x" != "x${DSP_PATCH}" ]; then
 DSP_PATCH="-open ${DSP_PATCH}"
fi

${PD} ${GUI_AUDIO} ${GUI_MIDI} ${GUI_OPTIONS} ${PD_OPTIONS} ${GUI_PATH} ${PD_PATH} ${GUI_LIB} ${PD_LIB} ${GUI_PATCH} -send "${GUI_MESSAGE}" -send "${PD_MESSAGE}" &

${PD} ${DSP_AUDIO} ${DSP_MIDI} ${DSP_OPTIONS} ${PD_OPTIONS} ${DSP_PATH} ${PD_PATH} ${DSP_LIB} ${PD_LIB} ${DSP_PATCH} -send "${DSP_MESSAGE}" -send "${PD_MESSAGE}"
