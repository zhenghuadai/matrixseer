LIBOBJ  = lib3Dhttp.so lib3Dsmtp.so #lib3Dfont.so
LIBA    = lib3Dhttp.a lib3Dsmtp.a #lib3Dfont.a 
TOPDIR=../../
PROJ=network
TARGET=liba
EXEC_TARGET=sendip
include ../../include/makefile.inc 


#sendip:sendip.cpp
#	${CC} ${CFLAG} ${CDEBFLAG} -o ${BINPATH}$@ -L${LIBPATH}   -l3Dhttp -l3Dsmtp -lpthread $^

lib:libso liba
liba: ${LIBA}
libso:${LIBOBJ} 
lib3Dhttp.so: httpsocket.o
	${CC} ${CFLAG}  -shared -Wl,-soname,$@ -o ${LIBPATH}$@ $^
lib3Dsmtp.so: smtpsocket.o ../io/iofile.o #readFile.o
	${CC} ${CFLAG}  -shared -Wl,-soname,$@ -o ${LIBPATH}$@ $^
lib3Dfont.so: ${FONTOBJ}
	${CC}  ${CFLAG} -shared -Wl,-soname,$@ -o ${LIBPATH}$@ $^
lib3Dhttp.a : ${OBJ_DIR}/httpsocket.o
	ar ${ARFLAG} ${LIBPATH}$@ $< 
lib3Dsmtp.a: ${OBJ_DIR}/smtpsocket.o  ../io/iofile.o #readFile.o
	ar ${ARFLAG} ${LIBPATH}$@ $^
lib3Dfont.a: ${FONTOBJ}
	ar ${ARFLAG} ${LIBPATH}$@ $^

$(OBJ_DIR)/httpsocket.o:httpsocket.cpp 
	g++ -c ${MARC} -fPIC -I../../include -o $@ $<
$(OBJ_DIR)/smtpsocket.o:smtpsocket.cpp
	g++ -c ${MARC} -fPIC -I../../include -o $@ $<
	
include ../../include/rules.mk	
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
clean0:
	rm -f ${TestEXEC}
	rm -f *.o htmlparser/*.o
	rm -f ${LIBPATH}lib*

