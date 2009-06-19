TOPDIR=../../../
include ${TOPDIR}/include/makefile.inc 

sendip:sendip.o
	${CC} ${CFLAG} ${CDEBFLAG} -o ${BINPATH}$@ -L${LIBPATH} ${SORDLIB} -l3Dhttp -l3Dsmtp -lpthread $^
testhttpsocket:testhttpsocket.o
	${CC} ${CFLAG} ${CDEBFLAG} -o ${BINPATH}$@ -L${LIBPATH} ${SORDLIB} -l3Dhttp -lpthread $^
testsmtpsocket:testsmtpsocket.o
	${CC} ${CFLAG}  ${CDEBFLAG}  -o ${BINPATH}$@ -L${LIBPATH}  ${SORDLIB} -l3Dsmtp $^
%.exe:%.o
	${CXX} ${CFLAG}  ${CDEBFLAG}  -o $@ -L${LIBPATH}   ${SORDLIB} ../smtpsocket.o ../../io/iofile.o  $^
	
include  ${TOPDIR}/include/rules.mk	
clean:
	rm -f ${TestEXEC}
	rm -f *.o htmlparser/*.o
	rm -f ${LIBPATH}lib*

