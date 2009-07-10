TOPDIR=../../
PROJ=fltkDisplay
XLIB	=   -lfltk2 -lXext -lX11 -lXi
DMDISPLAY =fltkDisplay
SRCS= display.cpp   ${FONTOBJ} ctrinput.cpp  ctrbutton.cpp  mainWindow.cpp font.cpp  ctrA.cpp ctrP.cpp ctrDiv.cpp cssBox.cpp ctrScroll.cpp ctrText.cpp #../network/readFile.cpp 
TARGET= 3DIE_ALL
include ${TOPDIR}/include/makefile.inc
OBJECTS =${MULTIMEDIA_OUT_DIR}/bmp.o ${NET_OUT_DIR}/httpsocket.o ${NET_OUT_DIR}/smtpsocket.o  ${OBJS}  ${IO_OUT_DIR}/iofile.o ${JS_OUT_DIR}/JSDocument.o



.phony:3DIE_ALL preBuild 
3DIE_ALL: preBuild 3DIE 
preBuild: 
	cd ../network;make
	cd ../js;make
	cd ../io;make
	cd ../multimedia;make

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

