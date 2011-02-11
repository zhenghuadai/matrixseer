CC = icc
sufix= sse2
OPTI = -O2
dflag = D__POUT
OBJ = alloc.o   dump_state.o  idct.o         mpeg2dec.o decode.o  header.o      motion_comp.o  slice.o commondzh.o idct_mmx.o motion_comp_sse2.o
mpeg2dec: ${OBJ}
	${CC} -o $@-${CC}-${sufix}-${OPTI} ${OPTI} $^
%.o:%.c
	${CC} -c -I/home/zhdai/include ${OPTI} $^
clean: 
	rm -rf *.o mpeg2dec
