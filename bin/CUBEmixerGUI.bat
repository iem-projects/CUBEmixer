@echo off

set PD_PATCH=../lib/GUI/GUI16+OUT.pd
set PD_INSTALL=..\src\pd\bin
set PD_AUDIO=-noaudio
set PD_MIDI=-nomidi
set PD_OPTIONS=-font 10
set PD_PATH=-path ..;../lib;../lib/abs/;../lib/libs/;../lib/plugins/;../lib/extensions/;../lib/GUI/;../lib/DSP/;../lib/GUI/abs;../lib/DSP/abs;../lib/libs/iemabs;../lib/libs/zexyabs
set PD_LIB=-lib zexy -lib iemlib1 -lib iemlib2 -lib iemmatrix -lib iem_ambi -lib iemgui

REM ***** overwrites optionally the PD_INSTALL variable *********
if exist LocalVars.bat (call LocalVars.bat)

@echo starting pd ...
%PD_INSTALL%\pd %PD_AUDIO% %PD_MIDI% %PD_OPTIONS% %PD_PATH% %PD_LIB% %PD_PATCH%
