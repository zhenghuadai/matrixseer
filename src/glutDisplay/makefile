CC	= g++
CPP	= g++
CFLAG	= -DXP_UNIX -g -DDZHP 
LIBPATH	= ../../lib/
BINPATH	= ../../bin/
INCLUDEPATH = -I../../include/ -I./  -I../ -I../htmlparser/ -I../js/include/ -I../network/ -I../js
XLIB	= -lGL -lglut
TestEXEC= testhttpsocket testsmtpsocket testfont
EXEC	=
LIBOBJ  = lib3Dhttp.so lib3Dsmtp.so lib3Dfont.so
LIBA    = lib3Dhttp.a lib3Dsmtp.a lib3Dfont.a libjs.a
SORDLIB = -WI,-Bdynamic #static
ARFLAG  = cqs
FONTOBJ = font.o glut_tr10.o glut_tr24.o glut_hel10.o   glut_hel12.o   glut_hel18.o   glut_mroman.o  glut_roman.o ../js/JSDocument.o
OBJECTS = display.o bmp.o ../network/httpsocket.o ../network/smtpsocket.o  ${FONTOBJ} ctrinput.o Rect.o ctrbutton.o ../io/iofile.o 
3DIE:3DIE.o ${OBJECTS}  ${LIBPATH}libhtml.a ../js/libjs.a
	cd ../network;make
	cd ../js;make
	cd ../io;make
	${CPP} ${CFLAG} -o ${BINPATH}$@   ${XLIB} $^ -lpthread
test: ${TestEXEC}
testhttpsocket:testhttpsocket.o
	${CC} ${CFLAG} -o ${BINPATH}$@ -L${LIBPATH} ${SORDLIB} -l3Dhttp -lpthread $^
testsmtpsocket:testsmtpsocket.o
	${CC} ${CFLAG}  -o ${BINPATH}$@ -L${LIBPATH}  ${SORDLIB} -l3Dsmtp $^
testfont:testfont.o
	${CC} ${CFLAG}  -o ${BINPATH}$@  ${SORDLIB} -l3Dfont -L${LIBPATH} -WI,-Bdynamic ${XLIB} $^
lib:libso liba
liba: ${LIBA}
libso:${LIBOBJ} #lib3Dhttp.so lib3Dsmtp.so lib3Dfont.so
#lib:lib3Dhttp.so lib3Dsmtp.so lib3Dfont.so ${LIBA}
lib3Dhttp.so: httpsocket.o
	${CC} ${CFLAG}  -shared -Wl,-soname,$@ -o ${LIBPATH}$@ $^
lib3Dsmtp.so: smtpsocket.o
	${CC} ${CFLAG}  -shared -Wl,-soname,$@ -o ${LIBPATH}$@ $^
lib3Dfont.so: ${FONTOBJ}
	${CC}  ${CFLAG} -shared -Wl,-soname,$@ -o ${LIBPATH}$@ $^
lib3Dhttp.a : httpsocket.o
	ar ${ARFLAG} ${LIBPATH}$@ $< 
lib3Dsmtp.a: smtpsocket.o
	ar ${ARFLAG} ${LIBPATH}$@ $<
lib3Dfont.a: ${FONTOBJ}
	ar ${ARFLAG} ${LIBPATH}$@ $^
${LIBPATH}libhtml.a:
	cd ../htmlparser;make
%.o:%.c
	${CC}  ${CFLAG}  ${INCLUDEPATH} -c $<
%.o:%.cpp
	${CPP}  ${CFLAG}  ${INCLUDEPATH} -c $<
install:
	if ! test  -f /usr/local/lib3Dhttp.so; then  \
	ln -s ${LIBPATH}httpsocket.so /usr/local/lib3Dhttp.so; \
	fi;
	if ! test  -f /usr/local/smtpsocket.so; then  \
	ln -s ${LIBPATH}httpsocket.so /usr/local/lib3Dsmtp.so; \
	fi;
	if ! test  -f /usr/local/lib3Dfont.so; then  \
	ln -s ${LIBPATH}httpsocket.so /usr/local/lib3Dfont.so; \
	fi;
clean:
	rm -f ${TestEXEC}
	rm -f *.o htmlparser/*.o
	rm -f ${LIBPATH}lib*

