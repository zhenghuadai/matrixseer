TOPDIR= ../..
PROJ=glutdisplay
DMDISPLAY =glutDisplay
EXEC	=

FONT_SRC= font.cpp glut_tr10.c glut_tr24.c glut_hel10.c   glut_hel12.c   glut_hel18.c   glut_mroman.c  glut_roman.c 
SRCS= ${FONT_SRC} display.cpp ctrinput.cpp Rect.cpp Cube.cpp ctrbutton.cpp displayDraw.cpp 3DIE.c 
#FONTOBJ = font.o glut_tr10.o glut_tr24.o glut_hel10.o   glut_hel12.o   glut_hel18.o   glut_mroman.o  glut_roman.o ../js/JSDocument.o
TARGET= 3DIE
include ../../include/makefile.inc 
OBJECTS =  ../multimedia/bmp.o ${NET_OUT_DIR}/httpsocket.o ${NET_OUT_DIR}/smtpsocket.o  ${OBJS}  ../io/iofile.o ../js/JSDocument.o


3DIE: ${OBJECTS}  ${LIBPATH}libhtml.a ../js/libjs.a
	cd ../network;make
	cd ../js;make
	cd ../io;make
	${CXX} ${MARC} ${CFLAG} -o ${BINPATH}$@   ${LIB} ${XLIB} $^ -lpthread
testfont:testfont.o
	${CC} ${CFLAG}  -o ${BINPATH}$@  ${SORDLIB} -l3Dfont -L${LIBPATH} -WI,-Bdynamic ${XLIB} $^
lib3Dfont.a: ${FONTOBJ}
	ar ${ARFLAG} ${LIBPATH}$@ $^
${LIBPATH}libhtml.a:
	cd ../htmlparser; DMDISPLAY=${DMDISPLAY} make
include ../../include/rules.mk
clean:
	rm -f *.o htmlparser/*.o
	rm -f ${LIBPATH}lib*

