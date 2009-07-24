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
#include "display.h"
void Widget::setNext(Widget* o)
{
	this -> mNext = o;
	if(o)
		o -> mPrevious = this;
}

int Widget::getSx()
{
	int ax=x();
	Widget* p = mParent;
	while(p){
		ax += p -> x();
		p = p -> parent();
	}
	return ax;
}

int Widget::getSy()
{
	int ay=y();
	Widget* p = mParent;
	while(p){
		ay += p -> y();
		p = p -> parent();
	}
	return ay;
}


int Widget::getSz()
{
	int az=z();
	Widget* p = mParent;
	while(p){
		az += p -> z();
		p = p -> parent();
	}
	return az;
}


void Widget::getSxyz(int& sx, int& sy, int& sz)
{
	int ax=x(), ay=y(), az=z();
	Widget* p = mParent;
	printf(" %d +", ay);
	while(p){
		ax += p -> x();
		ay += p -> y();
		az += p -> z();
		printf("+ %d ", p -> y());
		p = p -> parent();
	}
	sx = ax;
	sy = getWinh() - ay;
	sz = az;
	printf("= %d ", ay);
	printf("{%d (%d - %d =%d) %d}\n",sx,getWinh(),ay, sy, sz);
}
