#!/bin/sh

#ECHO=echo

CUBEMIXERPATH=$(dirname $0)/..

# source functions
. ${CUBEMIXERPATH}/lib/functions.sh

cd ${CUBEMIXERPATH}/tools/configurator/ || (echo "directory for CONFIGURATOR not found!!"; exit -1)

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

CONFIGURATOR_PATCH=MAIN.pd

${ECHO} ${PD} ${PD_OPTIONS} ${PD_PATH} ${PD_LIB} -path ../../lib/libs:abs -lib zexy:folder_list -open ${CONFIGURATOR_PATCH}
