#include "Cube.h"
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "debprintf.h"
Cube::Cube()
{
	debprintf("create0 :::::: z :%d \n", mZ);
}
Cube::Cube(int x,int y,int z)
{
    mX=x;
    mY=y;
    mZ=z;
	debprintf("create1 :::::: z :%d \n", mZ);
}
Cube::Cube(int x,int y,int z,int dx,int dy)
{
    mX=x;
    mY=y;
    mZ=z;
    mW=dx;
    mH=dy;
	debprintf("create2:::::: z :%d \n", mZ);
	debprintf("create2:::::: %d %d %d %d %d \n", mX, mY ,mZ, mW ,mH);
}
Cube::Cube(int dx,int dy)
{
    mW=dx;
    mH=dy;
	debprintf("create3:::::: z :%d \n", mZ);
}
void Cube::setwh(int dx,int dy)
{
    mW=dx;
    mH=dy;
}
void Cube::setxyz()
{
    int curRasterPos[4];
    //glGetIntegerv(GL_CURRENT_RASTER_POSITION,curRasterPos);
    getCurrentRasterPos(curRasterPos);
    mX=curRasterPos[0];
    mY=curRasterPos[1]-2;
    mZ=curRasterPos[2];
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


