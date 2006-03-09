@echo off

set LOCALVARS=../etc/CUBEmixer/LocalVars.bat

set PD_INSTALL=..\src\pd\bin
set PD_OPTIONS=-font 10
set PD_PATH=-path ..;../lib;../lib/abs/;../lib/libs/;../lib/plugins/;../lib/extensions/;../lib/GUI/;../lib/DSP/;../lib/GUI/abs;../lib/DSP/abs;../lib/libs/iemabs;../lib/libs/zexyabs
set PD_LIB=-lib zexy;iemlib1;iemlib2;iemmatrix;iem_ambi;iemgui

set GUI_PATCH=../lib/GUI/GUI16+OUT.pd
set GUI_AUDIO=-noaudio
set GUI_MIDI=-nomidi


REM ***** overwrites optionally the PD_INSTALL variable *********
if exist LocalVars.bat (call LocalVars.bat)

@echo starting pd ...
%PD_INSTALL%\pd %GUI_AUDIO% %GUI_MIDI% %GUI_OPTIONS% %PD_OPTIONS% %GUI_PATH% %PD_PATH% %GUI_LIB% %PD_LIB% %GUI_PATCH%
