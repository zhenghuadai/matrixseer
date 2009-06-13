CC= g++
CFLAG= -g  -DXP_UNIX -g -DDZHP -DDEB -DDZHDEB  
LIBPATH=../../lib/
BINPATH=../../bin/
INCLUDEPATH= -I../js/include -I../../include
DMDISPLAY ?=glutDisplay
DISPLAYPATH= ../${DMDISPLAY}
OBJ    = html.o tagfunc.o stack.o createtree.o attrfunc.o cssfunc.o htmltree.o
all:${LIBPATH} libhtml

libhtml:${OBJ}
	ar cqs  ${LIBPATH}libhtml.a $^
html: htmlmain.o  ${OBJ}
	${CC} ${CFLAG} -D__HTMLMAIN -c tagfunc.cpp
	${CC} ${CFLAG} -D__HTMLMAIN  -I./ -I../ -I${INCLUDEPATH}  -c ../JSDocument.cpp -o JSDocument.o
	${CC} ${CFLAG} -I./ -I../ -I${INCLUDEPATH} -o iofile.o -c ../iofile.c
	${CC} ${CFLAG} -I./ -I../ -I${INCLUDEPATH} -o httpsocket.o -c ../httpsocket.c
	${CC} ${CFLAG} -o ${BINPATH}$@  JSDocument.o $^ ../libjs.a httpsocket.o iofile.o -lpthread
#	${CC} ${CFLAG} -o ${BINPATH}$@ ${LIBPATH}libhtml.a  -L../../lib $<
createtree.cpp:${DISPLAYPATH}/ctrP.h
%.o:%.cpp
	${CC} ${CFLAG} -I./ -I../ ${INCLUDEPATH} -I${DISPLAYPATH}  -I../js  -c $<
libhtml.a: html.o tagfunc.o stack.o
	ar cqs  ${LIBPATH}$@ $^
${LIBPATH}:
	mkdir -p $@
clean:
	rm -f ${BINPATH}html
	rm -f *.o
