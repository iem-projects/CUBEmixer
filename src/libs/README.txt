Installtion of needed pd-externals for Cubemixer:
=================================================
(since version 1.0.1)

A) Overview pd-externals needed for CUBEmixer:
----------------------------------------------

aconnect (linux with alsamidi)
folder_list
hcs
hdspm_mixer
iem_ambi
iem_ambi2
iem_bin_ambi
iemgui
iemlib
iemmatrix
iem_spec2
miXed
net
osc
pdogg (optional)
popup
zexy

they will be installed in lib/libs/ 
(just copy everything in it without directory-hierarchy to this path)

All of these externals (with the notable exception of folder_list) 
are not included with this package but as external svn reference!

The missing externals are available from the pure-data SVN-repository 
hosted by SourceForge.
You can get them via svn, see sourceforge prject page of PD:

   svn co https://pure-data.svn.sourceforge.net/svnroot/pure-data/trunk/externals

B) QUICKINSTALL
---------------

Linux 
...........................

To ease the installation, you can run "make" in this directory.
This should compile all needed externals and install them into
a place where CUBEmixer will look for them.
Since some externals rely on the full pd-sources, you will have to give the
full path to your pd sources.
Use something like
sh~>make PDSOURCE=/path/to/sources/of/pd/src

If you use the pd included in the parent directory use the the absolute path to there.

WINDOWS:
........
(not tested with Version 3.0)
get the compiled package from http://sourceforge.net/projects/iem/ (files) and copy the content of lib/libs to the lib/libs folder.

Mac-OSX:
........
read the README_OSX.txt


iem, 23032006
updated for Version 3.0 20042009, winfried ritsch

