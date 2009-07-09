$(DBG_DIR)/%.o:%.c
	${CC}  ${MARC}  ${CFLAG}  ${INCLUDEPATH} -c -o $@ $<
${DBG_DIR}/%.o:%.cpp
	${CXX} ${MARC}  ${CFLAG}  ${INCLUDEPATH} -c -o $@ $<
${OPT_DIR}/%.o:%.cpp
	${CXX} ${MARC}  ${CFLAG}  ${INCLUDEPATH} -c -o $@ $<
${LIBPATH}:
	mkdir -p $@
createDir:
	@test -x ${DBG_DIR} || mkdir -p ${DBG_DIR}
clean:
	rm -rf ${OBJS} ${CLEAN_OBJS} 
