${DBG_DIR}/%.o:%.c
	${CC}  ${MARC}  ${CFLAG}  ${INCLUDEPATH}  -MMD -c -o $@ $<
	@cp $(DBG_DIR)/$*.d $(DEP_DIR)/$*.dep; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < $(DBG_DIR)/$*.d >> \
			$(DEP_DIR)/$*.dep; \
		rm -f $(DBG_DIR)/$*.d

${DBG_DIR}/%.o:%.cpp
	${CXX} ${MARC}  ${CFLAG}  ${INCLUDEPATH}  -MMD -c -o $@ $<
	@cp $(DBG_DIR)/$*.d $(DEP_DIR)/$*.dep; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < $(DBG_DIR)/$*.d >> \
			$(DEP_DIR)/$*.dep; \
		rm -f $(DBG_DIR)/$*.d

${OPT_DIR}/%.o:%.c
	${CC}  ${MARC}  ${CFLAG}  ${INCLUDEPATH}  -MMD -c -o $@ $<
	@cp $(OPT_DIR)/$*.d $(DEP_DIR)/$*.dep; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < $(OPT_DIR)/$*.d >> \
			$(DEP_DIR)/$*.dep; \
		rm -f $(OPT_DIR)/$*.d

${OPT_DIR}/%.o:%.cpp
	${CXX} ${MARC}  ${CFLAG}  ${INCLUDEPATH}  -MMD -c -o $@ $<
	@cp $(OPT_DIR)/$*.d $(DEP_DIR)/$*.dep; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < $(OPT_DIR)/$*.d >> \
			$(DEP_DIR)/$*.dep; \
		rm -f $(OPT_DIR)/$*.d

%.o:%.c
	${CC}  ${MARC}  ${CFLAG}  ${INCLUDEPATH}  -MMD -c -o $@ $<
%.o:%.cpp
	${CXX} ${MARC}  ${CFLAG}  ${INCLUDEPATH}  -MMD -c -o $@ $<
${LIBPATH}:
	@mkdir -p $@
-include ${DEP_FILES} 
createDir:
	@test -x ${DBG_DIR} || mkdir -p ${DBG_DIR}
	@test -x ${DEP_DIR} || mkdir -p ${DEP_DIR}
clean:
	rm -rf ${OBJS} ${CLEAN_OBJS} 
