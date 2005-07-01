#!/bin/sh


if [ "x$1" = "x" ]; then
  echo you have to specify a user !!!
  exit -1
fi


for i in `find . -name Root`
do
 echo $i
 TEMPFILE=`tempfile`
 sed -e "s/ext:.*@cvs/ext:$1@cvs/g" $i > $TEMPFILE
 mv $TEMPFILE $i
done
