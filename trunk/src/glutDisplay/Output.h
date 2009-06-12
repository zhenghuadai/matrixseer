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
class Output: public Widget
{
	public:
	Output(int x, int y, int z, int w, int h):Widget(x,y,z,w,h){}
	Output(int x, int y, int w, int h, const char* l):Widget(x,y,w,h,l){}

	void type(int t){}
	void textfont(int f){}
	void textsize(int f){}
	char* value() {return NULL;}
	void value(const char* l){}
};
#endif
