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
void Widget::addChild(Widget* child_)
{
	if(mChild) {
		appandChild(child_);
	}else{
		addFirstChild(child_);
	}
}

void Widget::appandChild(Widget* child_)
{
	Widget* lastSib = mChild->mPrevious;
	lastSib -> setNext(child_);
	child_  -> setNext(mChild);
	child_->mParent = this;	
}

void Widget::setNext(Widget* o)
{
	this -> mNext = o;
	o -> mPrevious = this;
}

void Widget::addFirstChild(Widget* child_)
{
	this-> mChild = child_;
    mChild -> mNext = NULL;
    mChild -> mPrevious = mChild;
	child_->mParent = this;
}
