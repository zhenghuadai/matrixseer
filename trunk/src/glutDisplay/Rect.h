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
		int inArea(int ptx,int pty, int ptz );
		RectINLINE void getFocus();
		RectINLINE void loseFocus();
		RectINLINE int  onFocus();
	protected:
		int _x,_y,_z;
		int _w,_h;
		int focus;
};
#endif
