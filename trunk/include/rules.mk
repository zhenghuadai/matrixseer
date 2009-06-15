%.o:%.c
	${CC}  ${CFLAG}  ${INCLUDEPATH} -c $<
%.o:%.cpp
	${CXX}  ${CFLAG}  ${INCLUDEPATH} -c $<
${LIBPATH}:
	mkdir -p $@
