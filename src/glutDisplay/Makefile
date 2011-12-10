TOPDIR= ../..
PROJ=glutdisplay
DMDISPLAY =glutDisplay
#EXEC_TARGET	= 3DIE

FONT_SRC= font.cpp glut_tr10.c glut_tr24.c glut_hel10.c   glut_hel12.c   glut_hel18.c   glut_mroman.c  glut_roman.c 
SRCS= ${FONT_SRC}  displayDraw.cpp 3DIE.c globject.c #Widget.cpp Group.cpp ctrinput.cpp Rect.cpp Cube.cpp ctrbutton.cpp
TARGET= 3DIE_ALL
include ../../include/makefile.inc 
OBJECTS =  ${OBJS}  ${MULTIMEDIA_OUT_DIR}/bmp.o ${JS_OUT_DIR}/JSDocument.o

.phony:3DIE_ALL preBuild 
3DIE_ALL: preBuild 3DIE 
preBuild: 
	cd ../network;make
	cd ../js;make
	cd ../io;make
	cd ../multimedia;make
	cd ../htmlparser;make
	cd ../display;make
3DIE: ${OBJECTS}  
	${CXX} ${MARC} ${CFLAG} -o ${BINPATH}$@  $^ ${LIB} ${XLIB}  -lpthread
testfont:testfont.o
	${CC} ${CFLAG}  -o ${BINPATH}$@  ${SORDLIB} -l3Dfont -L${LIBPATH} -WI,-Bdynamic ${XLIB} $^
lib3Dfont.a: ${FONTOBJ}
	ar ${ARFLAG} ${LIBPATH}$@ $^
${LIBPATH}libhtml.a:
	cd ../htmlparser; DMDISPLAY=${DMDISPLAY} make
${LIBPATH}libDisplay.a:
	cd ../display;make

include ../../include/rules.mk

