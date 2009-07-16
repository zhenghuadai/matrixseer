#ifndef __Cube_header__
#define __Cube_header__
#define CubeINLINE 
class Cube {
	public:
		Cube();
		Cube(int x,int y,int z);
		Cube(int dx,int dy);
		Cube(int x,int y,int z,int dx,int dy);
		CubeINLINE void	setxyz(int x,int y,int z);
		CubeINLINE void	setwh(int dx,int dy);
		CubeINLINE void setxyz();
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
		int inArea(int ptx,int pty, int ptz );
		CubeINLINE void getFocus();
		CubeINLINE void loseFocus();
		CubeINLINE int  onFocus();
	protected:
		int _x,_y,_z;
		int _w,_h,_d;
		int focus;
};
#endif