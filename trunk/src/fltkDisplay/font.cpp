#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "font.h"
//#include <FL/Fl.H>
#include <fltk/compat/FL/fl_draw.H>
#include <fltk/x.h>
#include <fltk/compat/FL/x.H>
//#include "Fl_Font.H"
#include "dmtype.h"
//#include "Fl_Font.H"
#include "dmtype.h"
#include "debprintf.h"
#include "display.h"
#define UPPERFONT 0
extern void forwardx(int x);
extern void getCurrentRasterPos(int *pos);
int charWidth = 12;
static GLuint fontOffset;
extern int rowHeight;
extern int getCurRectW();
int rowMargin = 5;

void dm_drawString(char * s,int n,int x,int y)
{
//    printf("draw(%d %d %s\n",x,y,s);
    fl_draw(s,n,x,y + rowHeight -rowMargin);
}

void setdefaultfont(void)
{
    fl_font(FL_TIMES,14);
}

INLINE int getCharWidth(char c)
{
    return (int) fl_width(c);
}

int getCharsWidth(char *str,int n)
{
    if (!fl_xfont) return -1.0;
    XCharStruct* p = fl_xfont->per_char;
    if (!p) return strlen(str)*fl_xfont->min_bounds.width;
    int a = fl_xfont->min_char_or_byte2;
    int b = fl_xfont->max_char_or_byte2 - a;
    int w = 0;
    while ((*str)&&(n--)) {
        int x = *(uchar*)str++ - a;
        if (x >= 0 && x <= b) w += p[x].width;
        else w += fl_xfont->min_bounds.width;
    }
    return w;
}
int getCharsWidth(char *str)
{
//    return (int) fl_width(str,strlen(str));

    if (!fl_xfont) return -1.0;
    XCharStruct* p = fl_xfont->per_char;
    if (!p) return strlen(str)*fl_xfont->min_bounds.width;
    int a = fl_xfont->min_char_or_byte2;
    int b = fl_xfont->max_char_or_byte2 - a;
    int w = 0;
    while (*str) {
        int x = *(uchar*)str++ - a;
        if (x >= 0 && x <= b) w += p[x].width;
        else w += fl_xfont->min_bounds.width;
    }
    return w;
}

/*
 *give box width, return the strlen in the box
 *
 * */
int getLenInW(char *str,int width)
{
    //setdefaultfont();
    width -= 10;
    debprintf("width:%d\n",width);
    if (!fl_xfont) return -1.0;
    char * strE = str;
    XCharStruct* p = fl_xfont->per_char;
    if (!p) {
     return width/ fl_xfont->min_bounds.width;
     }
    int a = fl_xfont->min_char_or_byte2;
    int b = fl_xfont->max_char_or_byte2 - a;
    int w = 0;
    while ((*strE)&&(w<width)&&(*strE != '\n')) {
        debprintf("%c",*strE);
        int x = *(uchar*)strE++ - a;
        if (x >= 0 && x <= b) w += p[x].width;
        else w += fl_xfont->min_bounds.width;
    //    printf("[%d %c %d]\n",w,*(strE-1),p[x].width);
    }
    debprintf("[%d]\n",w);
    //printf("%d %d %s \n",width,w,str);
    if(w >= width) {
        char * strEtmp = strE;
        strE --;
        while((strE > str) &&( *strE != ' ')){
        debprintf("%c",*strE);
        strE --;
        }
        if(strE == str) strE = strEtmp;
    }
    /*
    */
    debprintf("\n");
    return (int)(strE - str);
}
int getLenInW(char *str,double width)
{
    return getLenInW(str,(int)width);
}

/*
 * give width , return rows of the text
 */
int getHinW(char *str,int width)
{
    if (!fl_xfont) return -1.0;
    char * strE = str;
    int h=0;
    XCharStruct* p = fl_xfont->per_char;
    if (!p) {
     return (strlen(str)* fl_xfont->min_bounds.width -1) /width + 1;
     }
    int a = fl_xfont->min_char_or_byte2;
    int b = fl_xfont->max_char_or_byte2 - a;
    int w = 0;
    while ((*strE)) {
        if(*strE =='\n'){
            h += ((w-1)/width +1);
            w = 0;
            strE++;
            continue;
        }
        int x = *(uchar*)strE++ - a;
        if (x >= 0 && x <= b) w += p[x].width;
        else w += fl_xfont->min_bounds.width;
    }
   return ( h + (w-1)/width +1);
}

int2 getstrWH(char *str,int width)
{
    int2 wh;
    wh.x=wh.y=0;
    if (!fl_xfont) return wh;
    //width -=10;
    if(width <=0) return wh;
    char * strE = str;
    XCharStruct* p = fl_xfont->per_char;
    if (!p) {
        wh.y=(strlen(str)* fl_xfont->min_bounds.width -1) /width + 1;
        if(wh.y==1) wh.x = (strlen(str)* fl_xfont->min_bounds.width -1);
        else wh.x = width;
        return wh; 
    }
    int a = fl_xfont->min_char_or_byte2;
    int b = fl_xfont->max_char_or_byte2 - a;
    int w = 0;
    while ((*strE)) {
        if(*strE =='\n'){
            wh.y += ((w-1)/width +1);
            if(w>width) wh.x =w;
            else{
                if(wh.x<(w+FirstLineIndent))wh.x =w+FirstLineIndent;
            }
            w = 0;
            strE++;
            continue;
        }
        int x = *(uchar*)strE++ - a;
        if (x >= 0 && x <= b) w += (p[x].width);
        else w += fl_xfont->min_bounds.width;
                //printf("[%d %c %d]\n",w,*(strE-1),p[x].width);
    }
    //w -= (1*(strE -str)/8); // correct the width;
    wh.y += (w-1)/width +1;
    if(w > width) wh.x =width;
    else{
        if(wh.x<w+FirstLineIndent)wh.x =w+FirstLineIndent;
    }
    //if(wh.x>width) wh.x = width;
    //if(wh.x<4) wh.x =4;
    //    printf("[%d %d %s %d %d]\n",wh.x,wh.y,str,width,w);
    return wh;
}
int2 getstrWH(char *str)
{
    int width = getCurRectW();//mDC_w;
    return getstrWH(str,width);
}
void drawStringxy(int x,int y,char *s)
{
    dm_drawString(s,strlen(s),x,y);
    int xw = getCharsWidth(s);
    forwardx(xw);
}
void drawChars(char *s,int len)
{
    int pos[4];
    getCurrentRasterPos(pos);
    dm_drawString(s,len,pos[0],pos[1]);
#if 0
    char buf[4096];
    strncpy(buf,s,len);
    buf[len]=0;
    printf("\n{%s}\n",s);
    printf("%d\n",len);
    printf("[%d,%d,%s]\n",pos[0],pos[1],buf);
#endif
    int xw = getCharsWidth(s,len);
#if 0
    printf("%d\n",xw);
#endif
    forwardx(xw);
}

void drawString(char *s)
{
    int pos[4];
    getCurrentRasterPos(pos);
    dm_drawString(s,strlen(s),pos[0],pos[1]);
    int xw = getCharsWidth(s);
    forwardx(xw);
}

void drawCharsxy(char *s,int len,int x, int y)
{
    dm_drawString(s,len,x,y);
    int xw = getCharsWidth(s,len);
    forwardx(xw);
}

void drawCharsxyz(int x,int y,int z,char *s,int len)
{
}

void drawStringxyz(int x,int y,int z,char *s)
{
}
int getEnterPos(char * str,int n)
    /*when return, str[i]==0 or str[i]=='\n' or i==n
     *
     */
{
    int i=0;
    while((str[i])&&(i<n)&&(str[i] != '\n')) i++;
    return i;

}

int getEnterPos3(char * str,int n,int *pos)
{
    int i=0;
    while((str[i])&&(i<n)&&(str[i] != '\n')) i++;
    *pos=i;
    if(!str[i])return -1;
    if(i==n)return 1;
    return 0;
}


