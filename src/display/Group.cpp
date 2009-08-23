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
	child_ ->mNext = 0;
	mChild -> mPrevious = child_;
	child_->parent(this) ;	
}

void Group::addFirstChild(Widget* child_)
{
	this-> mChild = child_;
    mChild -> mNext = NULL;
    mChild -> mPrevious = mChild;
	child_->parent ( this);
}

Widget* Group::getObjOnFocus(int x, int y, int z)
{
	//if( hitMe(x, y, z) == HIT_NO) return NULL;
    debprintf("hit test Group:%0x\n: %d %d %d in? %d %d %d %d\n", this, x, y, z, 
    this->x(), this->y(), this->w(),this->h());
	Widget* o = child();
	while( o){
		if( o-> hitMe(x, y, z) == HIT_YES) 
			return o->getObjOnFocus(x, y, z);
		o = o -> next();	
	}
	return this;
}
