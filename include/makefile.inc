CC  = g++
CXX = g++
CPP	= g++
LINK= g++

#set arch
arch=$(shell uname -m)
ifeq "$(arch)" "x86_64"
MARC ?= #-m32 
LIBJS = libjs32.a
else 
MARC ?=  
LIBJS = libjs64.a
endif

#set PATH
OUT_DIR 	= ${TOPDIR}/builds/${PROJ}
DBG_DIR 	= ${OUT_DIR}/dbg
OPT_DIR 	= ${OUT_DIR}/opt
DEP_DIR 	= ${OUT_DIR}/.dep
OBJ_DIR     = ${DBG_DIR}
CFLAG  		=  -DXP_UNIX -DDZHP ${MARC} ${MY_FLAG} 
DBGCFLAG    = ${CFLAG} 
DBGCFLAG    += -g #  -DDEB -DDZHDEB    
OPTCFLAG    = ${CFLAG}
LIBPATH		= ${TOPDIR}/lib/
BINPATH		= ${TOPDIR}/bin/
VIRDISPLAY  = display

DISPLAYPATH = ${TOPDIR}/src/${DMDISPLAY}/
VIRDISPLAYPATH = ${TOPDIR}/src/${VIRDISPLAY}/
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

INCLUDEPATH += -I${TOPDIR}/include/  -I${PARSERPATH} -I${JSPATH}/include -I${NETWORKPATH} -I${JSPATH} -I${JSPATH}/include  -I${DISPLAYPATH} -I${IOPATH} -I${MEDIAPATH} -I${VIRDISPLAYPATH}

#set source file 
C_SRCS  	= $(SRCS:%.cpp=%.c)
DEP_FILES 	= $(C_SRCS:%.c=$(DEP_DIR)/%.dep)	
DBG_OBJS    = $(C_SRCS:%.c=$(DBG_DIR)/%.o)
OPT_OBJS    = $(C_SRCS:%.c=$(OPT_DIR)/%.o)

OBJS 		= ${DBG_OBJS}
#OBJS 		= ${OPT_OBJS}


#set lib
XLIB	+= -lGL -lglut
LIBOBJ  ?= lib3Dhttp.so lib3Dsmtp.so lib3Dfont.so
LIB     ?= -L${LIBPATH} -l3Dhttp -l3Dsmtp #lib3Dfont.a $(LIBJS)
SORDLIB = -WI,-Bdynamic #static
ARFLAG  = cqs


#set clean
ifdef TARGET
DBG_TARGET  ?=${TARGET}
OPT_TARGET  ?=${TARGET}
endif

ifdef LIBTARGET
DBG_LIBTARGET  ?=${LIBTARGET}d.a
OPT_LIBTARGET  ?=${LIBTARGET}.a
TARGET 		?=${DBG_LIBTARGET}
DBG_TARGET  ?=${DBG_LIBTARGET}
OPT_TARGET  ?=${OPT_LIBTARGET}
endif

ifndef TARGET
TARGET		?=${OBJS}
DBG_TARGET  ?=${TARGET}
OPT_TARGET  ?=${TARGET}
endif


CLEAN_OBJS 	?=${TARGET}


#make entry
.PHONY: all
all: createDirdbg ${TARGET} 
dbg: createDirdbg ${DBG_TARGET} 
opt: createDiropt ${OPT_TARGET} 
testecho:
	echo ${TARGET} 
	echo ${DBG_TARGET} 
	echo ${OPT_TARGET} 

