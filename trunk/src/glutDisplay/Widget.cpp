/*
 * =====================================================================================
 *
 *       Filename:  Widget.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/15/2009 08:53:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gamil.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include "Widget.h"
void Widget::setNext(Widget* o)
{
	this -> mNext = o;
	o -> mPrevious = this;
}


