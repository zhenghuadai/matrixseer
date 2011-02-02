#ifndef __CubemHeader__
#define __CubemHeader__
#define CubeINLINE 
#define HIT_YES 1
#define HIT_NO 0
#include "Pointer.h"
#include "display.h"
template<typename T>
class Cube {
    protected:
        T mX,mY,mZ; //! left-top
        T mW,mH,mD;
        int focus;
    public:
        Cube();
        Cube(T x,T y,T z);
        Cube(T dx,T dy);
        Cube(T x,T y,T z,T dx,T dy);
        Cube(T x, T y, T z, T w, T h, T d):mX(x), mY(y), mZ(z), mW(w), mH(h),mD(d){}
        void setwh(T dx,T dy);
        void setxyz(T x,T y,T z){mX=x;mY=y;mZ=z;}
        void setxyz();

        void x(T x0){mX= x0;}
        void y(T y0){mY= y0;}
        void z(T z0){mZ= z0;}
        void w(T w0){mW= w0;}
        void h(T h0){mH= h0;}
        void d(T d0){mD= d0;}

        void stepx(T x0){mX+= x0;}
        void stepy(T y0){mY+= y0;}
        void stepz(T z0){mZ+= z0;}

        T x(){return mX;}
        T y(){return mY;}
        T z(){return mZ;}
        T w(){return mW;}
        T h(){return mH;}
        T d(){return mD;}
        void position(T x, T y) {mX=x; mY=y;}
        //void setxyz(T x,T y, T z=0){position(x,y);}
        void setx(T x0){x(x0);}
        void sety(T y0){y(y0);}
        void setWH(T w0,T h0){w(w0);h(h0);}
        void size(T w0,T h0){w(w0);h(h0);}
        CubeINLINE void getFocus();
        CubeINLINE void loseFocus();
        CubeINLINE T  onFocus();
        int isPinCube(Pointer& p, Cube* c);
        int isPinRect(T x, T y, T z, T x0, T y0, T z0, T w, T h, T d){
            if( (( x > x0 ) && ( x< x0+w) )  && 
                    (( y < y0 ) && ( y> y0-h) )  && /*left top w h */
                    (( z > z0 ) && ( z< z0+d) ) 	)
                return HIT_YES;
            else return HIT_NO;
        }
        int isPinRect(T x, T y, T x0, T y0, T w, T h){
            if( (( x > x0 ) && ( x< x0+w) )  && 
                    (( y < y0 ) && ( y> y0-h) )     )
                return HIT_YES;
            else return HIT_NO;
        }

};
#include "Cube.cpp"
#endif
