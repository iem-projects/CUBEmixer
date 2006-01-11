#!/bin/sh

BUGS=BUGS.txt
TODO=TODO.txt

cat ${BUGS} | \
 egrep "^([[:alnum:]]|([[:space:]]*\-))" | \
 sed "/^[[:alnum:]]/{x;p;x}" \
 > ${TODO}
