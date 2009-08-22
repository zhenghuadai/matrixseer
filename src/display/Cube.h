#ifndef __CubemHeader__
#define __CubemHeader__
#define CubeINLINE 
#define HIT_YES 1
#define HIT_NO 0
#include "Pointer.h"
#include "display.h"
class Cube {
    protected:
        int mX,mY,mZ;
        int mW,mH,mD;
        int focus;
    public:
        Cube();
        Cube(int x,int y,int z);
        Cube(int dx,int dy);
        Cube(int x,int y,int z,int dx,int dy);
        Cube(int x, int y, int z, int w, int h, int d):mX(x), mY(y), mZ(z), mW(w), mH(h),mD(d){}
        void setwh(int dx,int dy);
        void setxyz(int x,int y,int z){mX=x;mY=y;mZ=z;}
        void setxyz();
        void x(int x0){mX= x0;}
        void y(int y0){mY= y0;}
        void z(int z0){mZ= z0;}
        void w(int w0){mW= w0;}
        void h(int h0){mH= h0;}
        void d(int d0){mD= d0;}

        int x(){return mX;}
        int y(){return mY;}
        int z(){return mZ;}
        int w(){return mW;}
        int h(){return mH;}
        int d(){return mD;}
        void position(int x, int y) {mX=x; mY=y;}
        //void setxyz(int x,int y, int z=0){position(x,y);}
        void setx(int x0){x(x0);}
        void sety(int y0){y(y0);}
        void setWH(int w0,int h0){w(w0);h(h0);}
        void size(int w0,int h0){w(w0);h(h0);}
        CubeINLINE void getFocus();
        CubeINLINE void loseFocus();
        CubeINLINE int  onFocus();
        int isPinCube(Pointer& p, Cube* c);
        int isPinRect(int x, int y, int z, int x0, int y0, int z0, int w, int h, int d){
            if( (( x > x0 ) && ( x< x0+w) )  && 
                    (( y < y0 ) && ( y> y0-h) )  && /*left top w h */
                    (( z > z0 ) && ( z< z0+d) ) 	)
                return HIT_YES;
            else return HIT_NO;
        }
        int isPinRect(int x, int y, int x0, int y0, int w, int h){
            if( (( x > x0 ) && ( x< x0+w) )  && 
                    (( y < y0 ) && ( y> y0-h) )     )
                return HIT_YES;
            else return HIT_NO;
        }

};
#endif
