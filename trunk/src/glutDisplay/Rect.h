#ifndef __Rect_header__
#define __Rect_header__
#define RectINLINE 
class Rect {
	public:
		Rect();
		Rect(int x,int y,int z);
		Rect(int dx,int dy);
		Rect(int x,int y,int z,int dx,int dy);
		RectINLINE void	setxyz(int x,int y,int z);
		RectINLINE void	setwh(int dx,int dy);
		RectINLINE void setxyz();
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
		int inArea(int ptx,int pty, int ptz );
		RectINLINE void getFocus();
		RectINLINE void loseFocus();
		RectINLINE int  onFocus();
	protected:
		int _x,_y,_z;
		int _w,_h,_d;
		int focus;
};
#endif
