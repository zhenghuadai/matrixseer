#include "Cube.h"
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
Cube::Cube()
{
	printf("create0 :::::: z :%d \n", _z);
}
Cube::Cube(int x,int y,int z)
{
    _x=x;
    _y=y;
    _z=z;
	printf("create1 :::::: z :%d \n", _z);
}
Cube::Cube(int x,int y,int z,int dx,int dy)
{
    _x=x;
    _y=y;
    _z=z;
    _w=dx;
    _h=dy;
	printf("create2:::::: z :%d \n", _z);
	printf("create2:::::: %d %d %d %d %d \n", _x, _y ,_z, _w ,_h);
}
Cube::Cube(int dx,int dy)
{
    _w=dx;
    _h=dy;
	printf("create3:::::: z :%d \n", _z);
}
void Cube::setwh(int dx,int dy)
{
    _w=dx;
    _h=dy;
}
void Cube::setxyz()
{
    int curRasterPos[4];
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,curRasterPos);
    _x=curRasterPos[0];
    _y=curRasterPos[1]-2;
    _z=curRasterPos[2];
}
CubeINLINE void Cube::getFocus()
{
    focus = 1;
}
CubeINLINE void Cube::loseFocus()
{
    focus = 0;
}
CubeINLINE int Cube::onFocus()
{
    return focus ;
}

CubeINLINE int Cube::inArea(int ptx,int pty, int ptz )
{
    if((ptx > _x)&&(ptx < _x + _w) && (pty > _y)&&(pty < _y + _h)) return 1;
    return 0;
}

