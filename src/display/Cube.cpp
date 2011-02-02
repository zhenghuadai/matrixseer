#include "Cube.h"
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "debprintf.h"
#define defineCubeHeader(type) template<typename T> type Cube<T>

template<typename T> 
Cube<T>::Cube()
{
	debprintf("create0 :::::: z :%d \n", mZ);
}

template<typename T> 
Cube<T>::Cube(T x,T y,T z)
{
    mX=x;
    mY=y;
    mZ=z;
	debprintf("create1 :::::: z :%d \n", mZ);
}

template<typename T> 
Cube<T>::Cube(T x,T y,T z,T dx,T dy)
{
    mX=x;
    mY=y;
    mZ=z;
    mW=dx;
    mH=dy;
	debprintf("create2:::::: z :%d \n", mZ);
	debprintf("create2:::::: %d %d %d %d %d \n", mX, mY ,mZ, mW ,mH);
}

template<typename T> 
Cube<T>::Cube(T dx,T dy)
{
    mW=dx;
    mH=dy;
	debprintf("create3:::::: z :%d \n", mZ);
}

template<typename T> 
void Cube<T>::setwh(T dx,T dy)
{
    mW=dx;
    mH=dy;
}

template<typename T> 
void Cube<T>::setxyz()
{
    T curRasterPos[4];
    //glGetIntegerv(GL_CURRENT_RASTER_POSITION,curRasterPos);
    getCurrentRasterPos(curRasterPos);
    mX=curRasterPos[0];
    mY=curRasterPos[1]-2;
    mZ=curRasterPos[2];
}

template<typename T> 
CubeINLINE void Cube<T>::getFocus()
{
    focus = 1;
}

template<typename T> 
CubeINLINE void Cube<T>::loseFocus()
{
    focus = 0;
}

template<typename T> 
CubeINLINE T Cube<T>::onFocus()
{
    return focus ;
}


