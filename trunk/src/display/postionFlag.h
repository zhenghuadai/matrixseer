/*
 * =====================================================================================
 *
 *       Filename:  postionFlag.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年05月24日 22时04分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */

#ifndef _POSITIONFLAG_H_
#define _POSITIONFLAG_H_
#define FIXEDPOS 3
#define FIXEDSIZE 24
#define FIXEDX 1
#define FIXEDY 2
#define FIXEDZ 4
#define FIXEDW 8
#define FIXEDH 16
#define FIXEDD 32
#define declarePFlag \
    public:\
int PostionFlag;

#define posFlagFunc \
    void setPostionFlag(int flag){PostionFlag = flag;    }\
void setFixX(int x0){x(x0);PostionFlag |= FIXEDX;}\
void setFixY(int y0){y(y0);PostionFlag |= FIXEDY;}\
void setFixXY(int x,int y){position(x,y);PostionFlag |= FIXEDPOS;}\
void setFixW(int w0){w(w0);PostionFlag |= FIXEDW;}\
void setFixH(int h0){h(h0);PostionFlag |= FIXEDH;}\
void setFixWH(int w,int h){size(w,h);PostionFlag |= FIXEDSIZE;}\
void setFixWH(){PostionFlag |= FIXEDSIZE;}\
int  isFixWH(){return PostionFlag & FIXEDSIZE ;}\
void setZeroPFlag(){PostionFlag = 0;}
#endif
