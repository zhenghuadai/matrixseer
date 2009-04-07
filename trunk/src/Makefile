CC	= gcc
CPP	= g++
CFLAG	= -g
LIBPATH	= ../lib/
BINPATH	= ../bin/
INCLUDEPATH = ../include/
XLIB	= -lGL -lglut
TestEXEC= testhttpsocket testsmtpsocket testfont
EXEC	=
LIBOBJ  = lib3Dhttp.so lib3Dsmtp.so lib3Dfont.so
LIBA    = lib3Dhttp.a lib3Dsmtp.a lib3Dfont.a
SORDLIB = -WI,-Bdynamic #static
ARFLAG  = cqs
OBJECTS = display.o bmp.o # font.o glut_tr10.o
FONTOBJ = font.o glut_tr10.o glut_tr24.o glut_hel10.o   glut_hel12.o   glut_hel18.o   glut_mroman.o  glut_roman.o
all: lib 3DIE 
3DIE:3DIE.o ${OBJECTS}
	${CPP} ${CFLAG} -o ${BINPATH}$@ -L${LIBPATH}  -l3Dhttp -l3Dsmtp -l3Dfont  ${XLIB} $^ 
test: ${TestEXEC}
testhttpsocket:testhttpsocket.o
	${CC} ${CFLAG} -o ${BINPATH}$@ -L${LIBPATH} ${SORDLIB} -l3Dhttp $^
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
%.o:%.c
	${CC}  ${CFLAG} -o $@ -I${INCLUDEPATH} -c $<
%.o:%.cpp
	${CPP}  ${CFLAG} -o $@ -I${INCLUDEPATH} -c $<
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
	rm -f *.o
	rm -f ${LIBPATH}lib*

