@echo off

set ETCDIR=../etc/CUBEmixer

set LOCALVARSTEMPLATE=%ETCDIR%/LocalVars.template.bat
set STARTUPPATCHES=%ETCDIR%/configured.bat
set LOCALVARS=%ETCDIR%/LocalVars.bat

REM default values
if exist %LOCALVARSTEMPLATE% (call %LOCALVARSTEMPLATE%)

REM ***** overrides the startup-patch *********
if exist %STARTUPPATCHES% (call %STARTUPPATCHES%)

REM ***** optionally overrides the PD_INSTALL variable *********
if exist %LOCALVARS% (call %LOCALVARS%)

echo starting pd ...
%PD_INSTALL%\pd %DSP_AUDIO% %DSP_MIDI% %DSP_OPTIONS% %PD_OPTIONS% %DSP_PATH% %PD_PATH% %DSP_LIB% %PD_LIB% %DSP_PATCH%

