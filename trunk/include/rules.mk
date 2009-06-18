%.o:%.c
	${CC} ${MARC}  ${CFLAG}  ${INCLUDEPATH} -c $<
%.o:%.cpp
	${CXX} ${MARC}  ${CFLAG}  ${INCLUDEPATH} -c $<
${LIBPATH}:
	mkdir -p $@
clean:
	rm -rf *.o
