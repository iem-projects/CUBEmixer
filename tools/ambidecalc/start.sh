#!/bin/sh

pd -path ../../lib/libs:../../lib/DSP/abs -lib iemmatrix:zexy:iem_ambi2:iemgui:iemlib1:iemlib2 -open ambidecigner.pd $@
