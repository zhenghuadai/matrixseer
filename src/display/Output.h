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
        char* mText;
		Color mTextColor;
    public:
        Output(int x, int y, int z, int w, int h ,const char*l):Widget(x,y,z,w,h,l),mText((char*)l){}
        Output(int x, int y, int w, int h, const char* l):Widget(x,y,w,h,l),mText((char*)l){}
        Output(int x, int y, int z, int w, int h, int d, const char* l):Widget(x,y,z,w,h,d,l),mText((char*)l){}

        void type(int t){}
        void textfont(int f){}
        void textsize(int f){}
		void textColor(Color c){mTextColor=c;}
		Color textColor(){return mTextColor;}
        char* value() {return mText;}
        char* text(){return mText;}
        void value(const char* l){mText = (char*)l;}
        virtual void draw(){  int sx,sy,sz; getSxyz(sx,sy,sz);putStrScr(sx,sy-getRowHeight(),sz,mText);debprintf("<%d %d %d> <%s> this->%0x next->%0x parent->%0x\n",sx,sy,sz,mText, this, this-> next(), this->parent());}
};
#endif
