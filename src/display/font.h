#ifndef MATRIXFONTH
#define MATRIXFONTH
#include "glutbitmap.h"
/*
extern const BitmapFontRec   MglutBitmapTimesRoman10;//MglutBitmapTimesRoman10;
#define MGLUT_BITMAP_TIMES_ROMAN_10 (void *)&MglutBitmapTimesRoman10;//MglutBitmapTimesRoman10;
*/
    extern void* MglutStrokeRoman;
    extern void* MglutStrokeMonoRoman;
    extern void* MglutBitmap9By15;
    extern void* MglutBitmap8By13;
    extern void* MglutBitmapTimesRoman10;
    extern void* MglutBitmapTimesRoman24;
    extern void* MglutBitmapHelvetica10;
    extern void* MglutBitmapHelvetica12;
    extern void* MglutBitmapHelvetica18;

    /*
 *      * Those pointers will be used by following definitions:
 *           */
#   define  MGLUT_STROKE_ROMAN               ((void *) &MglutStrokeRoman)
#   define  MGLUT_STROKE_MONO_ROMAN          ((void *) &MglutStrokeMonoRoman)
#   define  MGLUT_BITMAP_9_BY_15             ((void *) &MglutBitmap9By15)
#   define  MGLUT_BITMAP_8_BY_13             ((void *) &MglutBitmap8By13)
#   define  MGLUT_BITMAP_TIMES_ROMAN_10      ((void *) &MglutBitmapTimesRoman10)
#   define  MGLUT_BITMAP_TIMES_ROMAN_24      ((void *) &MglutBitmapTimesRoman24)
#   define  MGLUT_BITMAP_HELVETICA_10        ((void *) &MglutBitmapHelvetica10)
#   define  MGLUT_BITMAP_HELVETICA_12        ((void *) &MglutBitmapHelvetica12)
#   define  MGLUT_BITMAP_HELVETICA_18        ((void *) &MglutBitmapHelvetica18)

void defaultfont(void);
BitmapFontPtr selectfontfromid(void * id);
#define  MoveTo2(x,y) glRasterPos2i(x,y)
#define MoveTo3(x,y,z) glRasterPos2i(x,y,z)
#ifndef INLINE
#define INLINE //inline
#endif

#define FirstLineIndent 20
INLINE void MoveTo3(int x, int y,int z);
void drawChars(char *s,int len);
void drawString(char *s);
int getEnterPos(char * str,int n);
INLINE int getCharWidth(char c);
#endif
