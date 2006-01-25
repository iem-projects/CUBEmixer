#!/bin/sh


if [ "x$1" = "x" ]; then
  echo you have to specify a user !!!
  exit -1
fi


find . -name Root | while read i
do
 echo $i
 sed -i -e "s/ext:.*@cvs/ext:$1@cvs/g" "$i"
done
