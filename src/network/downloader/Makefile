LIBOBJ  = lib3Dhttp.so lib3Dsmtp.so #lib3Dfont.so
LIBA    = lib3Dhttp.a lib3Dsmtp.a #lib3Dfont.a 
TOPDIR=../../../
include ${TOPDIR}include/makefile.inc 

downer:webpage.o ../httpsocket.o smthBBS.o downer.o urlManager.o
	${CXX} -o $@ $^ -lpthread
	
include ${TOPDIR}include/rules.mk	

