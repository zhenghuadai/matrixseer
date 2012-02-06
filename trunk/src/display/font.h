#ifndef MATRIXFONTH
#define MATRIXFONTH
#include "dmtype.h"
#ifndef INLINE
#define INLINE //inline
#endif

#define FirstLineIndent 20
void defaultfont(void);
void drawChars(char *s,int len);
void drawString(char *s);
int  getEnterPos(char * str,int n);
int  fontCharWidth(char c);
int2 getWH_Chars(int w,int h,int id);
#endif
