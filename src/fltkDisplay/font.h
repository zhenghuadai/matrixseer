#ifndef MATRIXFONTH
#define MATRIXFONTH
#ifndef INLINE
#define INLINE //inline
#endif
INLINE void MoveTo3(int x, int y,int z);
void drawChars(char *s,int len);
void drawChars(char *s,int len,int x,int y);
void drawString(char *s);
int getEnterPos(char * str,int n);
INLINE int getCharWidth(char c);
void drawChars(char *s,int len);
int getLenInW(char *str,double width);
int getCharsWidth(char *str,int n);
void dm_drawString(char * s,int n,int x,int y);

#define GM_BLACK FL_BLACK
#endif
