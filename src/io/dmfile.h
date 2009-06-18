
#ifndef  DMFILE_HEADER__INC
#define  DMFILE_HEADER__INC
#ifdef __cplusplus
extern "C" {
#endif
int isfile(char *fn);
int is_fileexist(char *comm);
char * getsfromfile(char *fn);
#ifdef __cplusplus
}
#endif
#endif   /* ----- #ifndef DMFILE_HEADER__INC  ----- */
