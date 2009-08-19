#ifndef __Cube_header__
#define __Cube_header__
#define CubeINLINE 
#define HIT_YES 1
#define HIT_NO 0
#include "Pointer.h"
#include "display.h"
class Cube {
	public:
		Cube();
		Cube(int x,int y,int z);
		Cube(int dx,int dy);
		Cube(int x,int y,int z,int dx,int dy);
		void setwh(int dx,int dy);
		void setxyz(int x,int y,int z){_x=x;_y=y;_z=z;}
		void setxyz();
		void x(int x0){_x= x0;}
		void y(int y0){_y= y0;}
		void z(int z0){_z= z0;}
		void w(int w0){_w= w0;}
		void h(int h0){_h= h0;}
		void d(int d0){_d= d0;}

		int x(){return _x;}
		int y(){return _y;}
		int z(){return _z;}
		int w(){return _w;}
		int h(){return _h;}
		int d(){return _d;}
		void position(int x, int y) {_x=x; _y=y;}
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
		
	protected:
		int _x,_y,_z;
		int _w,_h,_d;
		int focus;
};
#endif
