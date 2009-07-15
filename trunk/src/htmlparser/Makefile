TOPDIR = ../..
PROJ=htmlparser
DMDISPLAY ?=glutDisplay
SRCS = html.cpp tagfunc.cpp stack.cpp createtree.cpp attrfunc.cpp cssfunc.cpp htmltree.cpp
TARGET=libhtml
CLEAN_OBJS= ${LIBPATH}libhtml.a

ifeq "$(DMDISPLAY)" "glutDisplay"
MY_FLAG= -DNEED_RENDER=n
endif

include ../../include/makefile.inc 

#all:${LIBPATH} init_dbg libhtml 

libhtml:${OBJS}
	ar cqs  ${LIBPATH}libhtml.a $^
html: htmlmain.o  ${OBJS}
	${CC} ${CFLAG} -D__HTMLMAIN -c tagfunc.cpp
	${CC} ${CFLAG} -D__HTMLMAIN  -I./ -I../ -I${INCLUDEPATH}  -c ../JSDocument.cpp -o JSDocument.o
	${CC} ${CFLAG} -I./ -I../ -I${INCLUDEPATH} -o iofile.o -c ../iofile.c
	${CC} ${CFLAG} -I./ -I../ -I${INCLUDEPATH} -o httpsocket.o -c ../httpsocket.c
	${CC} ${CFLAG} -o ${BINPATH}$@  JSDocument.o $^ ../libjs.a httpsocket.o iofile.o -lpthread
include ../../include/rules.mk	
