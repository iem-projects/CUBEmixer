#!/bin/sh

FORCE=
PLUGINDIR=plugins
PLUGINFILE=${PLUGINDIR}/plugins.txt

GUIPATCH=gui.pd
DSPPATCH=dsp.pd

function usage() {
 echo "$0 [-f] [-h] [<plugindir>]"
 exit 0
}

function parse_args() {
 while [ "$1" ]; do
  case "$1" in
  -h|--help)
    usage
    ;;
  -f|--force)
    FORCE="yes"
    ;;
  *)
    PLUGINDIR="$1"
    ;;
  esac
  shift
 done
}

if [ -d ${PLUGINDIR} ]; then
 :
else
 usage
fi

if [ -z "${GUIPATCH}" ]; then
 echo "no gui-patch defined!"
fi

if [ -z "${DSPPATCH}" ]; then
 echo "no dsp-patch defined!"
fi

function checkPlugin() {
 local d
 d=$1
  if [ -f ${d}/${GUIPATCH} ] && [ -f ${d}/${DSPPATCH} ]; then
    echo ${d#${PLUGINDIR}/}
  fi

}

function list_plugins() {
 local plugin
 find ${PLUGINDIR} -mindepth 1 -maxdepth 1 -type d | while read plugin; do
  checkPlugin $plugin
 done
}

function generateGUIpatch() {
  local plugin
  local plugfile
  local connector
  plugfile=$1
  if [ -r "${plugfile}" ]; then
    echo "#N canvas 0 0 450 300 10;"
    echo "#X obj 100 50 inlet;"
    echo "#X obj 100 75 route $(cat ${plugfile});"
    echo "#X connect 0 0 1 0;"
    let connector=2
    for plugin in $(cat ${plugfile}); do
      echo "#X obj 100 $[100+25*${connector}] ${plugin}/gui;"
      echo "#X connect 1 $[${connector}-2] ${connector} 0;"
      let connector++
    done
  fi
}


parse_args $@

if [ -e "${PLUGINFILE}" ] && [ -z "${FORCE}" ]; then
 echo "${PLUGINFILE} already exists!"
 echo "not overriding unless you specify \"-f\""
 echo
 exit -2
fi

if [ "${PLUGINFILE}" ]; then
 list_plugins > ${PLUGINFILE}

 generateGUIpatch ${PLUGINFILE}
 
else 
 list_plugins
fi



