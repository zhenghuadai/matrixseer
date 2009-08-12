#include "Cube.h"
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "debprintf.h"
Cube::Cube()
{
	debprintf("create0 :::::: z :%d \n", _z);
}
Cube::Cube(int x,int y,int z)
{
    _x=x;
    _y=y;
    _z=z;
	debprintf("create1 :::::: z :%d \n", _z);
}
Cube::Cube(int x,int y,int z,int dx,int dy)
{
    _x=x;
    _y=y;
    _z=z;
    _w=dx;
    _h=dy;
	debprintf("create2:::::: z :%d \n", _z);
	debprintf("create2:::::: %d %d %d %d %d \n", _x, _y ,_z, _w ,_h);
}
Cube::Cube(int dx,int dy)
{
    _w=dx;
    _h=dy;
	debprintf("create3:::::: z :%d \n", _z);
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


