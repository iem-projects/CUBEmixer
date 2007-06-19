#!/bin/sh

INFILE=speakerlayout.txt
OUTFILE=output.mtx


function parse_args() {

 while [ "$#" -ge 1 ]; do 
  case "x$1" in
  x--infile|x-i)
	INFILE=$2
	shift
	;;
  x--outfile|x-o)
	OUTFILE=$2
	shift
	;;
  *)
	arg=$1
	if [ "x${arg}" != "x${arg#--infile=}" ]; then INFILE=${arg#--infile=}; fi
	if [ "x${arg}" != "x${arg#-i=}" ]; then INFILE=${arg#-i=}; fi

	if [ "x${arg}" != "x${arg#--outfile=}" ]; then OUTFILE=${arg#--outfile=}; fi
	if [ "x${arg}" != "x${arg#-o=}" ]; then OUTFILE=${arg#-o=}; fi
	;;
  esac
  shift
 done

 if [ -r "${INFILE}" ]; then
   :
 else
   echo "input file \"${INFILE}\" not readable!!"
   exit -1
 fi

 if [ -e "${OUTFILE}" ]; then
   echo "output file \"${OUTFILE}\" already exists!!"
   echo "please delete manually before proceeding..."
   exit -2
 fi
}


parse_args $@

echo "infile: ${INFILE}"
echo "outfile: ${OUTFILE}"

pd -nogui -path ../../lib/libs:../../lib/DSP/abs:. -lib iemmatrix:zexy:iem_ambi:iemlib1:iemlib2 -open MAIN.pd -send ";inoutfile ${INFILE} ${OUTFILE}; pd quit"
