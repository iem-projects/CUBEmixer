OGGAMP - Plugin for CUBEMixer 
Version: 1.0RC1
IEM - winfried ritsch

OGGAMP is an extension to receive multichannel ogg/vorbis streams over network.
Each output of the oggstream can be choosen by the Channels strips and some other inputs.
OGGAMP was derived from SFPLAYER16 extension

Parameter:

 - hostname or IP: the name of the host or IP-number to fetch the stream from
 - streamname: the name of the stream
 - port number: the port number of the stream

Controls:

 - play, stop
 - R: recover, fill buffer again
 - RR: reconnect to URL and recover

Indicators:

 - status of stream if connected
 - time how long the stream has been running (measures time of connection state ON)

Description:
   OGGAMP extension can be use dmultiple, selecting different names as first arguments in for
   multiple instances in DSP and GUI. 
   Place youre GUI under the extension subwindow or main CUBEmixer GUI.

Requirements:

   the pdogg externals are needed for this extension, (should be included in the src folder)

NOTE: tested on linux system to work stable.

DOWNLOAD: part of CUBEmixer 
   in https://iem.svn.sourceforge.net/svnroot/iem/spatialization/CUBEmixer/


Changelog:

2009-02-20: initial version prototyped

