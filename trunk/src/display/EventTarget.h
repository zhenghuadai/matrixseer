/*
 * =====================================================================================
 *
 *       Filename:  EventTarget.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/10/2009 21:38:23 AM
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
#include "Cube.h"
class EventTarget: public Cube
{
	public:
	EventTarget(){init();};
	EventTarget(int x,int y,int z):Cube(x,y,z){init();};
	EventTarget(int dx,int dy):Cube(dx,dy){init();};
	EventTarget(int x,int y,int z,int dx,int dy,const char* l):Cube(x,y,z,dx,dy){init();};
	EventTarget(int x,int y,int dx,int dy , const char* l):Cube(x,y,0,dx,dy){init();};
	private:
	void	init(){}
};