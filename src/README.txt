Here all sources go in used for building the CUBEmixer.

Optionally PD can be installed locally in the CUBEmixer Folder for Distribution:
 in <CUBEMIXER>/lib/pd
for this just execute "make" in this dir, this will also build the needed externals

Without building externals execute "make pd-locinstall" or just build it "make pd-build"
see Makefile for more Details.

Note:
External Libraries are placed and will be build in the libs/ folder with an additional Makefile
See the README in there. There are optional externals, that have to be build and installed there.

20090420, winfried ritsch


