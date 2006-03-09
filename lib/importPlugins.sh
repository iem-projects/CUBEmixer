#!/bin/sh

FORCE=
PLUGINDIR=plugins
PLUGINFILE=${PLUGINDIR}/plugins.txt

GUIPATCH=GUI.pd
DSPPATCH=DSP.pd

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
  local pluginlist
  plugfile=$1
  pluginlist="";
  for plugin in $(cat ${plugfile}); do
      pluginlist="${pluginlist} ${plugin}"
  done
  if [ -r "${plugfile}" ]; then
    echo "#N canvas 0 0 450 300 10;"
    let connector=0
    for plugin in ${pluginlist}; do
      echo "#X obj 60 $[100+25*${connector}] inchan/plugin/plugin ${plugin} \\\$1 \\\$2;"
      let connector++
    done
  fi
}

function generateDSPpatch() {
  local plugin
  local plugfile
  local connector
  local pluginlist
  plugfile=$1
  pluginlist="";
  for plugin in $(cat ${plugfile}); do
      pluginlist="${pluginlist} ${plugin}"
  done
  if [ -r "${plugfile}" ]; then
    echo "#N canvas 0 0 450 300 10;"
    let connector=0
    for plugin in ${pluginlist}; do
      echo "#X obj 60 $[100+25*${connector}] inchan/plugin/plugin ${plugin} \\\$1 \\\$2;"
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

# generateGUIpatch ${PLUGINFILE}
 generateDSPpatch ${PLUGINFILE}

else 
 list_plugins
fi



