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

typedef struct 
{
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
}Event;
