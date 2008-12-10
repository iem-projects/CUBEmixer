#!/bin/sh

cd  ${0%%/*}

pd -path ../../lib/libs/:abs -lib zexy MAIN.pd
