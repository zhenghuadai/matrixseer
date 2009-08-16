TOPDIR= ../..
PROJ=glutdisplay
DMDISPLAY =glutDisplay
EXEC	=

FONT_SRC= font.cpp glut_tr10.c glut_tr24.c glut_hel10.c   glut_hel12.c   glut_hel18.c   glut_mroman.c  glut_roman.c 
SRCS= ${FONT_SRC} display.cpp ctrinput.cpp Rect.cpp Cube.cpp ctrbutton.cpp displayDraw.cpp 3DIE.c Widget.cpp Group.cpp 
TARGET= 3DIE_ALL
include ../../include/makefile.inc 
OBJECTS =  ${MULTIMEDIA_OUT_DIR}/bmp.o ${NET_OUT_DIR}/httpsocket.o ${NET_OUT_DIR}/smtpsocket.o  ${OBJS}  ${IO_OUT_DIR}/iofile.o ${JS_OUT_DIR}/JSDocument.o

.phony:3DIE_ALL preBuild 
3DIE_ALL: preBuild 3DIE 
preBuild: 
	cd ../network;make
	cd ../js;make
	cd ../io;make
	cd ../multimedia;make
	cd ../htmlparser;make
3DIE: ${OBJECTS}  ${LIBPATH}libhtml.a ../js/libjs.a
	${CXX} ${MARC} ${CFLAG} -o ${BINPATH}$@   ${LIB} ${XLIB} $^ -lpthread
testfont:testfont.o
	${CC} ${CFLAG}  -o ${BINPATH}$@  ${SORDLIB} -l3Dfont -L${LIBPATH} -WI,-Bdynamic ${XLIB} $^
lib3Dfont.a: ${FONTOBJ}
	ar ${ARFLAG} ${LIBPATH}$@ $^
${LIBPATH}libhtml.a:
	cd ../htmlparser; DMDISPLAY=${DMDISPLAY} make
include ../../include/rules.mk

