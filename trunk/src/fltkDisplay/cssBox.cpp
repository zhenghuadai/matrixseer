#include <stdio.h>
#include <fltk/Box.h>
#include <fltk/Style.h>
#include <fltk/draw.h>
#include "cssBox.h"
using namespace fltk;

#define SIZE 3
LineStyle setLineStyle(int lstyle,float lwidth,const char *ldashes)
{
    LineStyle oldLineStyle ;
    oldLineStyle.lineStyle_ = line_style_;
    oldLineStyle.lineWidth_ = line_width_;
    oldLineStyle.lineDashes_= line_dashes_;
    line_style(lstyle,lwidth,ldashes);
    return oldLineStyle;
}

void rollBackLineStyle(LineStyle oldLineStyle)
{
    line_style(oldLineStyle.lineStyle_ ,oldLineStyle.lineWidth_, \
              oldLineStyle.lineDashes_);
}
void CssBox:: _draw(const Rectangle& r1) const
{
    const Color bg = getbgcolor();
    const Color fg = getcolor();
    Rectangle r(r1); r.move_r(-SIZE); r.move_b(-SIZE);
//    printf("Box(%d %d %d %d)\n",r.x(),r.y(),r.w(),r.h());
    if (r.w() > 2 && r.h() > 2 && !drawflags(INVISIBLE)) {
        setcolor(bg);
        fillrect(r.x()+1,r.y()+1,r.w()-2,r.h()-2);
    }
    setcolor(GRAY33);
    fillrect(r.x()+SIZE, r.b(),  r.w(), SIZE);
    fillrect(r.r(), r.y()+SIZE, SIZE, r.h());
    fillrect(r.x()-SIZE, r.y(), SIZE, r.h());
    fillrect(r.x()-SIZE, r.y()-SIZE, r.w(),SIZE);
    //    setcolor(fg);
    setcolor(RED);
    LineStyle oldLineStyle = setLineStyle(2,1,0);
    strokerect(r);
    rollBackLineStyle(oldLineStyle);
}
static CssBox cssBox("css_box");
Box* CSS_BOX = &cssBox;

