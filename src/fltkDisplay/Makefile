TOPDIR=../../
include ${TOPDIR}/include/makefile.inc
XLIB	=   -lfltk2 -lXext -lX11 -lXi
DMDISPLAY =fltkDisplay

OBJECTS = display.o  ../network/httpsocket.o ../network/smtpsocket.o  ${FONTOBJ} ctrinput.o  ctrbutton.o ../io/iofile.o ../js/JSDocument.o mainWindow.o font.o ../multimedia/bmp.o ctrA.o ctrP.o ctrDiv.o cssBox.o ctrScroll.o ctrText.o #../network/readFile.o 

3DIE:3DIE.o ${OBJECTS}  ${LIBPATH}libhtml.a ../js/libjs.a
	${CPP} ${CFLAG} -o ${BINPATH}$@ $^  ${XLIB}  -lpthread
${LIBPATH}libhtml.a:
	cd ../htmlparser; DMDISPLAY=${DMDISPLAY} make
include ${TOPDIR}/include/rules.mk	
ctrP.o: ctrP.h	ctrP.cpp
ctrDiv.o: ctrDiv.h	ctrDiv.cpp
clean:
	rm -f ${TestEXEC}
	rm -f *.o htmlparser/*.o
	rm -f ${LIBPATH}lib*

