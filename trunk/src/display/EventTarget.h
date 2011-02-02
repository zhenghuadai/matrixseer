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
template<typename T=int>
class EventTarget: public Cube<T>
{
	public:
	EventTarget(){init();};
	EventTarget(T x,T y,T z):Cube<T>(x,y,z){init();};
	EventTarget(T dx,T dy):Cube<T>(dx,dy){init();};
	EventTarget(T x,T y,T z,T dx,T dy):Cube<T>(x,y,z,dx,dy){init();};
	EventTarget(T x,T y,T dx,T dy ):Cube<T>(x,y,0,dx,dy){init();};
    EventTarget(T x, T y, T z, T w, T h, T d):Cube<T>(x, y, z, w, h, d){init();}
	private:
	void	init(){}
};
