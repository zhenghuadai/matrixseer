LIBOBJ  = lib3Dhttp.so lib3Dsmtp.so #lib3Dfont.so
LIBA    = lib3Dhttp.a lib3Dsmtp.a #lib3Dfont.a 
TOPDIR=../../../
include ${TOPDIR}include/makefile.inc 

mailclient:mailclient.o mail_t.o mailclientmain.o
	${CXX} ${MARC} -o $@ $^ -I.. 
	
include ${TOPDIR}include/rules.mk	

