CC  = g++
CXX = g++
CPP	= g++
LINK= g++

#set arch
arch=$(shell uname -m)
ifeq "$(arch)" "x86_64"
MARC ?= -m32 
else 
MARC ?=  
endif

#set PATH
OUT_DIR 	= ${TOPDIR}/builds/${PROJ}
DBG_DIR 	= ${OUT_DIR}/dbg
OPT_DIR 	= ${OUT_DIR}/opt
OBJ_DIR     = ${DBG_DIR}
CFLAG  		= -g  -DXP_UNIX -g -DDZHP -DDEB -DDZHDEB  ${MARC}
LIBPATH		= ${TOPDIR}/lib/
BINPATH		= ${TOPDIR}/bin/

DISPLAYPATH = ${TOPDIR}/src/${DMDISPLAY}/
PARSERPATH 	= ${TOPDIR}/src/htmlparser/
JSPATH  	= ${TOPDIR}/src/js/
NETWORKPATH = ${TOPDIR}/src/network/
IOPATH 		= ${TOPDIR}/src/io
MEDIAPATH 	=${TOPDIR}/src/multimedia

DISPLAY_OUT_DIR=${TOPDIR}/builds/glutdisplay/dbg 
MULTIMEDIA_OUT_DIR=${TOPDIR}/builds/multimedia/dbg
NET_OUT_DIR=${TOPDIR}/builds/network/dbg
IO_OUT_DIR=${TOPDIR}/builds/io/dbg
JS_OUT_DIR=${TOPDIR}/builds/js/dbg

INCLUDEPATH += -I${TOPDIR}/include/  -I${PARSERPATH} -I${JSPATH}/include -I${NETWORKPATH} -I${JSPATH} -I${JSPATH}/include  -I${DISPLAYPATH} -I${IOPATH} -I${MEDIAPATH}

#set source file 
C_SRCS  = $(SRCS:%.cpp=%.c)
DBG_OBJS    = $(C_SRCS:%.c=$(DBG_DIR)/%.o)
OPT_OBJS    = $(C_SRCS:%.c=$(OPT_DIR)/%.o)
OBJS = ${DBG_OBJS}


#set lib
XLIB	= -lGL -lglut
LIBOBJ  ?= lib3Dhttp.so lib3Dsmtp.so lib3Dfont.so
LIB    ?= -L${LIBPATH} -l3Dhttp -l3Dsmtp #lib3Dfont.a libjs.a
SORDLIB = -WI,-Bdynamic #static
ARFLAG  = cqs


#set clean
TARGET ?=${OBJS}
CLEAN_OBJS ?=${TARGET}


#make entry
.PHONY: all
all: createDir ${TARGET} 
