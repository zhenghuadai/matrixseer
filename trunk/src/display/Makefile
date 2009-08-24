TOPDIR= ../..
PROJ=display
EXEC	=

SRCS= Widget.cpp Group.cpp ctrinput.cpp Rect.cpp Cube.cpp ctrbutton.cpp #display.cpp displayDraw.cpp 3DIE.c  
TARGET= libDisplay.a 
include ../../include/makefile.inc 

.phony:
libDisplay.a: ${OBJS}
	ar ${ARFLAG} ${LIBPATH}/$@ $^
include ../../include/rules.mk

