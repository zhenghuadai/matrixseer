CC	= g++
CPP	= g++
CFLAG	= -DXP_UNIX -g -DDZHP -DdownLoaderTest -DLINUX 
CDEBFLAG=-DDEB 
LIBPATH	= ../../lib/
BINPATH	= ../../bin/
INCLUDEPATH = -I../../include/ -I./  -I../ -I../htmlparser/ #-I/home/dzh/include/js/
XLIB	= -lGL -lglut
TestEXEC= testhttpsocket testsmtpsocket testfont
EXEC	=
LIBOBJ  = lib3Dhttp.so lib3Dsmtp.so lib3Dfont.so
LIBA    = lib3Dhttp.a lib3Dsmtp.a lib3Dfont.a #libjs.a
SORDLIB = -WI,-Bdynamic #static
ARFLAG  = cqs

all: liba 
# ${OBJECTS}  
sendip:sendip.o
	${CC} ${CFLAG} ${CDEBFLAG} -o ${BINPATH}$@ -L${LIBPATH} ${SORDLIB} -l3Dhttp -l3Dsmtp -lpthread $^
testhttpsocket:testhttpsocket.o
	${CC} ${CFLAG} ${CDEBFLAG} -o ${BINPATH}$@ -L${LIBPATH} ${SORDLIB} -l3Dhttp -lpthread $^
testsmtpsocket:testsmtpsocket.o
	${CC} ${CFLAG}  ${CDEBFLAG}  -o ${BINPATH}$@ -L${LIBPATH}  ${SORDLIB} -l3Dsmtp $^
testfont:testfont.o
	${CC} ${CFLAG}  -o ${BINPATH}$@  ${SORDLIB} -l3Dfont -L${LIBPATH} -WI,-Bdynamic ${XLIB} $^
lib:libso liba
liba: ${LIBA}
libso:${LIBOBJ} 
lib3Dhttp.so: httpsocket.o
	${CC} ${CFLAG}  -shared -Wl,-soname,$@ -o ${LIBPATH}$@ $^
lib3Dsmtp.so: smtpsocket.o readFile.o
	${CC} ${CFLAG}  -shared -Wl,-soname,$@ -o ${LIBPATH}$@ $^
lib3Dfont.so: ${FONTOBJ}
	${CC}  ${CFLAG} -shared -Wl,-soname,$@ -o ${LIBPATH}$@ $^
lib3Dhttp.a : httpsocket.o
	ar ${ARFLAG} ${LIBPATH}$@ $< 
lib3Dsmtp.a: smtpsocket.o readFile.o
	ar ${ARFLAG} ${LIBPATH}$@ $^
lib3Dfont.a: ${FONTOBJ}
	ar ${ARFLAG} ${LIBPATH}$@ $^

downer:webpage.o httpsocket.o smthBBS.o
	${CPP} -o $@ $^ -lpthread
mailclient:mailclient.o mail_t.o mailclientmain.o
	${CPP} -o $@ $^ -I.. 
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

