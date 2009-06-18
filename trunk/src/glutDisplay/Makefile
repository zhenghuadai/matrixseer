TOPDIR= ../../
DMDISPLAY =glutDisplay
EXEC	=
include ../../include/makefile.inc 
FONTOBJ = font.o glut_tr10.o glut_tr24.o glut_hel10.o   glut_hel12.o   glut_hel18.o   glut_mroman.o  glut_roman.o ../js/JSDocument.o
OBJECTS = display.o bmp.o ../network/httpsocket.o ../network/smtpsocket.o  ${FONTOBJ} ctrinput.o Rect.o ctrbutton.o ../io/iofile.o 
3DIE:3DIE.o ${OBJECTS}  ${LIBPATH}libhtml.a ../js/libjs.a
	cd ../network;make
	cd ../js;make
	cd ../io;make
	${CPP} ${MARC} ${CFLAG} -o ${BINPATH}$@   ${XLIB} $^ -lpthread
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

