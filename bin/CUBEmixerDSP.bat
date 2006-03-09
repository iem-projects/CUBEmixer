@echo off

set LOCALVARS=../etc/CUBEmixer/LocalVars.bat

set PD_INSTALL=..\src\pd\bin
set PD_OPTIONS=-font 10
set PD_PATH=-path ..;../lib;../lib/abs/;../lib/libs/;../lib/plugins/;../lib/extensions/;../lib/GUI/;../lib/DSP/;../lib/GUI/abs;../lib/DSP/abs;../lib/libs/iemabs;../lib/libs/zexyabs
set PD_LIB=-lib zexy;iemlib1;iemlib2;iemmatrix;iem_ambi;iemgui

set DSP_PATCH=..\lib\DSP\DSP+NET+MIDI+CUE_WIN.pd
set DSP_AUDIO=-r 44100 -audiobuf 80 -sleepgrain 10 -channels 26
set DSP_MIDI=

REM ***** overwrites optionally the PD_INSTALL variable *********
if exist %LOCALVARS% (call $LOCALVARS%)

@echo starting pd ...
%PD_INSTALL%\pd %DSP_AUDIO% %DSP_MIDI% %DSP_OPTIONS% %PD_OPTIONS% %DSP_PATH% %PD_PATH% %DSP_LIB %PD_LIB% %DSP_PATCH%
