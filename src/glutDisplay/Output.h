/*
 * =====================================================================================
 *
 *       Filename:  Group.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/09/2009 11:46:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gamil.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#ifndef __OUTPUT__HEADER__
#define __OUTPUT__HEADER__
#include <stdio.h>
#include <stdlib.h>
#include "Widget.h"
#define TIMES 1
#include "display.h"

class Output: public Widget
{
    private:
        char* text;
    public:
        Output(int x, int y, int z, int w, int h ,const char*l):Widget(x,y,z,w,h,l),text(0){}
        Output(int x, int y, int w, int h, const char* l):Widget(x,y,w,h,l),text(0){}

        void type(int t){}
        void textfont(int f){}
        void textsize(int f){}
        char* value() {return text;}
        void value(const char* l){text = (char*)l;}
        virtual void draw(){  int sx,sy,sz; getSxyz(sx,sy,sz);putStrScr(sx,sy,sz,text);printf("<%d %d %d> <%s>\n",x(),y(),z(),text);}
};
#endif
