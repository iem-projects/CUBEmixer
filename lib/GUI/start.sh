#!/bin/sh

PDPATHS="-path .:..:../abs/:../libs/:../plugins/:abs"
PDLIBS="-lib zexy:iemlib1:iemlib2:iemgui:iemmatrix"

pd $PDPATHS $PDLIBS $@
