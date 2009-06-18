LIBOBJ  = lib3Dhttp.so lib3Dsmtp.so #lib3Dfont.so
LIBA    = lib3Dhttp.a lib3Dsmtp.a #lib3Dfont.a 
TOPDIR=../../
include ../../include/makefile.inc 

sendip:sendip.o
	${CC} ${CFLAG} ${CDEBFLAG} -o ${BINPATH}$@ -L${LIBPATH} ${SORDLIB} -l3Dhttp -l3Dsmtp -lpthread $^
testhttpsocket:testhttpsocket.o
	${CC} ${CFLAG} ${CDEBFLAG} -o ${BINPATH}$@ -L${LIBPATH} ${SORDLIB} -l3Dhttp -lpthread $^
testsmtpsocket:testsmtpsocket.o
	${CC} ${CFLAG}  ${CDEBFLAG}  -o ${BINPATH}$@ -L${LIBPATH}  ${SORDLIB} -l3Dsmtp $^
testfont:testfont.o
	${CC} ${CFLAG}  -o ${BINPATH}$@  ${SORDLIB} -l3Dfont -L${LIBPATH} -WI,-Bdynamic ${XLIB} $^
	
include ../../include/rules.mk	
clean:
	rm -f ${TestEXEC}
	rm -f *.o htmlparser/*.o
	rm -f ${LIBPATH}lib*

