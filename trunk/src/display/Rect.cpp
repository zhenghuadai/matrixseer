#include "Rect.h"
#include <GL/gl.h>
Rect::Rect()
{
}
Rect::Rect(int x,int y,int z)
{
    _x=x;
    _y=y;
    _z=z;
}
Rect::Rect(int x,int y,int z,int dx,int dy)
{
    _x=x;
    _y=y;
    _z=z;
    _w=dx;
    _h=dy;
}
Rect::Rect(int dx,int dy)
{
    _w=dx;
    _h=dy;
}
void Rect::setwh(int dx,int dy)
{
    _w=dx;
    _h=dy;
}
void Rect::setxyz()
{
    int curRasterPos[4];
    //glGetIntegerv(GL_CURRENT_RASTER_POSITION,curRasterPos);
    getCurrentRasterPos(curRasterPos);
    _x=curRasterPos[0];
    _y=curRasterPos[1]-2;
    _z=curRasterPos[2];
}
void Rect::setxyz(int x,int y,int z)
{
    _x=x;
    _y=y;
    _z=z;
}
RectINLINE void Rect::getFocus()
{
    focus = 1;
}
RectINLINE void Rect::loseFocus()
{
    focus = 0;
}
RectINLINE int Rect::onFocus()
{
    return focus ;
}

RectINLINE int Rect::inArea(int ptx,int pty, int ptz )
{
    if((ptx > _x)&&(ptx < _x + _w) && (pty > _y)&&(pty < _y + _h)) return 1;
    return 0;
}

