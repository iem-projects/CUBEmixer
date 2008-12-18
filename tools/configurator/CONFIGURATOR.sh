#!/bin/sh

cd  ${0%%/*}

pd -nosound -nomidi -path ../../lib/libs/:abs -lib zexy MAIN.pd
