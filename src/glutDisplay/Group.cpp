/*
 * =====================================================================================
 *
 *       Filename:  Group.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/16/2009 11:11:37 AM
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
#include "Group.h"

void Group::addChild(Widget* child_)
{
	if(mChild) {
		appandChild(child_);
	}else{
		addFirstChild(child_);
	}
}

void Group::appandChild(Widget* child_)
{
	Widget* lastSib = mChild->mPrevious;
	lastSib -> setNext(child_);
	//child_  -> setNext(mChild);
	child_ ->mNext = 0;
	mChild -> mPrevious = child_;
	child_->setParent(mParent) ;	
}

void Group::addFirstChild(Widget* child_)
{
	this-> mChild = child_;
    mChild -> mNext = NULL;
    mChild -> mPrevious = mChild;
	child_->mParent = this;
}
