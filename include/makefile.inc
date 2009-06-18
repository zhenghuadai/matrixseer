CC  = g++
CXX = g++
CPP	= g++
LINK= g++
arch=$(shell uname -m)
ifeq "$(arch)" "x86_64"
MARC ?= -m32 
else 
MARC ?=  
endif

CFLAG = -g  -DXP_UNIX -g -DDZHP -DDEB -DDZHDEB  ${MARC}
LIBPATH	= ${TOPDIR}/lib/
BINPATH	= ${TOPDIR}/bin/
DISPLAYPATH = ${TOPDIR}/src/${DMDISPLAY}/
PARSERPATH= ${TOPDIR}/src/htmlparser/
JSPATH  = ${TOPDIR}/src/js/
NETWORKPATH= ${TOPDIR}/src/network/
IOPATH = ${TOPDIR}/src/io
INCLUDEPATH += -I${TOPDIR}/include/  -I${PARSERPATH} -I${JSPATH}/include -I${NETWORKPATH} -I${JSPATH} -I${JSPATH}/include  -I${DISPLAYPATH} -I${IOPATH}
XLIB	= -lGL -lglut
LIBOBJ  ?= lib3Dhttp.so lib3Dsmtp.so lib3Dfont.so
LIBA    ?= lib3Dhttp.a lib3Dsmtp.a lib3Dfont.a libjs.a
SORDLIB = -WI,-Bdynamic #static
ARFLAG  = cqs
