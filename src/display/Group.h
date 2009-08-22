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
#ifndef __GROUP__HEADER__
#define __GROUP__HEADER__
#include <stdio.h>
#include <stdlib.h>
#include "Widget.h"
class Group : public Widget
{
	private:
		Widget* mChild;
	public:
		Group(int x, int y, int z, int w, int h, const char* l):Widget(x,y,z,w,h,l),mChild(NULL){}
		Group(int x, int y, int w, int h, const char* l):Widget(x,y,w,h,l),mChild(NULL){}
        Group(int x, int y, int z, int w, int h, int d, const char* l):Widget(x, y, z, w, h, d,l),mChild(NULL){}
	public:
		virtual void addChild(Widget*);
		virtual Widget* getObjOnFocus(int x, int y, int z);
	public:
		Widget* child(){return mChild;}
	private:
		void appandChild(Widget* child_);
		void addFirstChild(Widget* child_);

};
#endif
