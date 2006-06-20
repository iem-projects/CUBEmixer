current: all

.SUFFIXES: .pd_linux

PDSOURCE=/usr/local/src/pd/src
PDDEST=../../lib/libs/

INCLUDE = -I. -I$(PDSOURCE)

LDFLAGS = -export-dynamic -shared
LIB = -ldl -lm -lpthread

#select either the DBG and OPT compiler flags below:

CFLAGS = -DPD -DUNIX -W -Werror -Wno-unused \
	-Wno-parentheses -Wno-switch -O6 -funroll-loops -fomit-frame-pointer \
        -DDL_OPEN

SYSTEM = $(shell uname -m)

# the sources

SRC = sigspec2_block_delay.c \
	sigspec2_math.c \
	sigspec2_tabreceive.c \
	sigspec2_tabreceive_enable.c \
	sigspec2_tab_conv.c \
	sigspec2_clip_max.c \
	sigspec2_clip_min.c \
	sigspec2_1p1z_time.c \
	sigspec2_1p1z_freq.c \
	sigspec2_log.c \
	sigspec2_stretch.c \
	sigspec2_shift.c \
	sigspec2_matrix_bundle_stat.c \
	iem_spec2.c

TARGET = iem_spec2.pd_linux


OBJ = $(SRC:.c=.o) 

#
#  ------------------ targets ------------------------------------
#

clean:
	rm ../../lib/$(TARGET)
	rm *.o

all: $(OBJ)
	@echo :: $(OBJ)
	ld $(LDFLAGS) -o $(TARGET) *.o $(LIB)
	strip --strip-unneeded $(TARGET)
	rm *.o
	
install:
	mv $(TARGET) ../../lib

$(OBJ) : %.o : %.c
	touch $*.c
	cc $(CFLAGS) $(INCLUDE) -c -o $*.o $*.c




