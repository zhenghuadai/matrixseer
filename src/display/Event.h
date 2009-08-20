/*
 *Copyright(C) by Zhenghua Dai. All rights reserved.
 *
 *
 *
 */
/*
 * =====================================================================================
 *
 *       Filename:  Event.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009年08月11日 20时12分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>

typedef enum _EVENT{
LBUTTON_UP   = 256,
LBUTTON_DOWN = 257,

}EVENT;
class Event
{
public:
	U32 msg; 
	U32 reserved; 
	union{
		U32 wPara;
		double wParaD;
	};
	union{
		U32 lPara;
		double lParaD;
	};
public:
    Event(){}
    Event(U32 aMsg, U32 aW, U32 aL):msg(aMsg),wPara(aW),lPara(aL){}
};

class ButtonEvent : public Event
{
public:
    ButtonEvent():Event(){}
    ButtonEvent(U32 aMsg, U32 aW, U32 aL):Event(aMsg,wPara,lPara){}
};
