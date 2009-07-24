#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>
#include <stdarg.h>
#include "font.h"
#include "bmp.h"
#include "html.h"
#include "display.h"
#include "app.h"
extern int charWidth;
extern int mDC_w;
extern int mDC_h;
extern int rowHeight ;

extern int2 fontSize;
extern dmRect curRasterPos[10];
extern dmRect curStartPos[10];
extern int curWin;
extern int2 cWH;
extern dmRect stackStartPos[20];
extern int topStartPos ;

extern int Tex_Win_h;
extern int Tex_Win_w;


void drawLine(double x,double y,double z,double x2,double y2,double z2)
{
	//glBegin(GL_LINE);
	glBegin(GL_LINE_STRIP);
	glVertex3d(x,y,z);
	glVertex3d(x2,y2,z2);
	glEnd();
}

void drawLine(double x,double y,double x2,double y2)
{
	glBegin(GL_LINE);
	glVertex3d(x,y,0.0);
	glVertex3d(x2,y2,.0);
	glEnd();
}

void drawLine( int x, int y, int z, int x2, int y2, int z2)
{
}

void drawFrame(double x,double y,double z,double dx,double dy)
{
	//   GLfloat oldcolor[4];
	//   glGetFloatv(GL_CURRENT_COLOR,oldcolor);
	glBegin(GL_LINE_STRIP);
	//    glColor4f(.8,.8,.8,1.0);
	glVertex3d(x,y,z);
	glVertex3d(x,y+dy,z);
	glVertex3d(x+dx,y+dy,z);
	glVertex3d(x+dx,y,z);
	glVertex3d(x,y,z);
	glEnd();
}

void drawFrame(int x, int y, int z, int dx, int dy)
{

}

void drawButton2dframe(double x,double y,double z,double dx,double dy)
{
	GLfloat oldcolor[4];
	glGetFloatv(GL_CURRENT_COLOR,oldcolor);
	glBegin(GL_LINE_STRIP);
	glColor4f(.8,.8,.8,1.0);
	glVertex3d(x,y,z);
	glVertex3d(x,y+dy,z);
	glVertex3d(x+dx,y+dy,z);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glColor4f(.1,.1,.1,1.0);
	glVertex3d(x,y,z);
	glVertex3d(x+dx,y,z);
	glVertex3d(x+dx,y+dy,z);
	glEnd();
	glColor4fv(oldcolor);
}
void drawButton2d(double x,double y,double z,double dx,double dy)
{
	GLfloat oldcolor[4];
	glGetFloatv(GL_CURRENT_COLOR,oldcolor);
	glBegin(GL_QUADS);
	glColor4f(.9,.9,.9,1.0);
	glVertex3d(x,y,z);
	glVertex3d(x+dx,y,z);
	glVertex3d(x+dx,y+dy,z);
	glVertex3d(x,y+dy,z);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glColor4f(.8,.8,.8,1.0);
	glVertex3d(x,y,z);
	glVertex3d(x,y+dy,z);
	glVertex3d(x+dx,y+dy,z);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glColor4f(.1,.1,.1,1.0);
	glVertex3d(x,y,z);
	glVertex3d(x+dx,y,z);
	glVertex3d(x+dx,y+dy,z);
	glEnd();
	glColor4fv(oldcolor);
}
void drawInput2d(double x,double y,double z,double dx,double dy)
{
	GLfloat oldcolor[4];
	glGetFloatv(GL_CURRENT_COLOR,oldcolor);
	glBegin(GL_QUADS);
	glColor4f(.3,.3,.3,3.0);
	glVertex3d(x,y,z);
	glVertex3d(x+dx,y,z);
	glVertex3d(x+dx,y+dy,z);
	glVertex3d(x,y+dy,z);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glColor4f(.1,.1,.1,1.0);
	glVertex3d(x,y,z);
	glVertex3d(x,y+dy,z);
	glVertex3d(x+dx,y+dy,z);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glColor4f(.8,.8,.8,1.0);
	glVertex3d(x,y,z);
	glVertex3d(x+dx,y,z);
	glVertex3d(x+dx,y+dy,z);
	glEnd();
	glColor4fv(oldcolor);
}
/*
   void drawInput(int dx,int dy)
   {
   glGetIntegerv(GL_CURRENT_RASTER_POSITION,curRasterPos[curWin]);
   drawInput2d(curRasterPosX,curRasterPosY+20,0,dx,dy);
   curRasterPosX += (dx + 1);
   MoveTo2(curRasterPosX,curRasterPosY);
   }
   */
int putStrScr(char * content,int width,int height)
	/*start from the left end
	 *
	 *
	 *
	 */
{
	int i=0,j=0;
	char *pstart=content;
	int tmpheight=height-20;
	cWH=getWH_Chars(width,height,3);

	if(!content)return 0;
	while(*pstart)
	{
		int npos=getEnterPos(pstart,cWH.w);
		glRasterPos2i(20, tmpheight-j*20);

		{
			char buf[20];
			sprintf(buf,"%d] ",tmpheight-j*20);
			drawChars(buf,strlen(buf));
		}

		drawChars(pstart,npos);
		pstart+=npos;
		if(*pstart=='\n')pstart++;
		j++;
	}
	return 1;
}
int putStrScr(char *content,int len)
	/*start from current pos
	 *
	 *
	 *
	 */
{
	int i=0,j=0;
	char *pstart=content;
	char *pend  = pstart+len;
	int npos;
	int2 tmpcWH;	
	if((!content)||(! *content))return 0;
	glGetIntegerv(GL_CURRENT_RASTER_POSITION,(GLint*)&curRasterPos[curWin]);
	//	printf("%d,%d\n",curRasterPosX,curRasterPosY);
	tmpcWH=getWH_Chars(Tex_Win_w-curRasterPosX,Tex_Win_h,3);
	npos=getEnterPos(pstart,tmpcWH.w);
	npos = npos < len ? npos:len;
	drawChars(pstart,npos);
	pstart+=npos;
	if(pstart == pend)return 1;
	if((*pstart=='\n')&&(! (*(pstart+1)))){
		curRasterPosY-= rowHeight;
		MoveTo2(20,curRasterPosY);
		return 1; 
	}

	while((*pstart) &&(pstart < pend))
	{
		curRasterPosY-= rowHeight;
		MoveTo2(20,curRasterPosY);
		npos=getEnterPos(pstart,cWH.w);
		npos = npos < len ? npos:len;
		drawChars(pstart,npos);
		pstart+=npos;
		if(pstart == pend) return 1;
		if(*pstart=='\n'){
			pstart++;
		}
	}
	return 1;
}
int putStrScr(char *content)
	/*start from current pos
	 *
	 *
	 *
	 */
{
	int i=0,j=0;
	char *pstart=content;
	int npos;

	int2 tmpcWH;	
	if((!content)||(! *content))return 0;
	glGetIntegerv(GL_CURRENT_RASTER_POSITION,(GLint*)&curRasterPos[curWin]);
	//	printf("%d,%d\n",curRasterPosX,curRasterPosY);
	tmpcWH=getWH_Chars(Tex_Win_w-curRasterPosX,Tex_Win_h,3);
	npos=getEnterPos(pstart,tmpcWH.w);
	drawChars(pstart,npos);
	pstart += npos;
	if((*pstart=='\n')&&(! (*(pstart+1)))){
		curRasterPosY-= rowHeight;
		MoveTo2(20,curRasterPosY);
		return 1; 
	}

	while(*pstart)
	{
		curRasterPosY-= rowHeight;
		MoveTo2(20,curRasterPosY);
		npos=getEnterPos(pstart,cWH.w);
		drawChars(pstart,npos);
		pstart+=npos;
		if(*pstart=='\n'){
			pstart++;
		}
	}
	return 1;
}

int putStrScr(int x, int y, int z, char *content)
	/*start from current pos
	 *
	 *
	 *
	 */
{
	int i=0,j=0;
	char *pstart=content;
	int npos;
	int2 tmpcWH;	




	if((!content)||(! *content))return 0;
    MoveTo2(x,y);
	glGetIntegerv(GL_CURRENT_RASTER_POSITION,(GLint*)&curRasterPos[curWin]);
	tmpcWH=getWH_Chars(Tex_Win_w-x ,Tex_Win_h,3);
	npos=getEnterPos(pstart,tmpcWH.w);
	printf("char num:%d ,x:%d  y:%d\n", npos,x,y);
	drawChars(pstart,npos);
	pstart += npos;
	if((*pstart=='\n')&&(! (*(pstart+1)))){
		curRasterPosY-= rowHeight;
		MoveTo2(20,curRasterPosY);
		return 1; 
	}

	while(*pstart)
	{
		curRasterPosY-= rowHeight;
		MoveTo2(20,curRasterPosY);
		npos=getEnterPos(pstart,cWH.w);
		printf("char num:%d ,x:%d  y:%d\n", npos,20, curRasterPosY);
		drawChars(pstart,npos);
		pstart+=npos;
		if(*pstart=='\n'){
			pstart++;
		}
	}
	return 1;
}


void drawButton(int dx,int dy,char * s)
{
	int startPos[4];
	int endPos[4];
	glGetIntegerv(GL_CURRENT_RASTER_POSITION,startPos);
	putStrScr(s);
	glGetIntegerv(GL_CURRENT_RASTER_POSITION,endPos);
	drawButton2d(startPos[0],startPos[1]-3,startPos[2],endPos[0] - startPos[0],22);
	endPos[0] += 1;
	MoveTo2(endPos[0],endPos[1]);
	//drawInput2d(startPos[0],startPos[1]+20,startPos[2],200,30);

}
int Disprintf(char * format,...)
{
	va_list ap;
	char *buf;
	buf=(char *) malloc(4096);
	va_start(ap,format);
	vsprintf(buf,format, ap);
	va_end(ap);
	putStrScr(buf);
	free(buf);
	return 1;
}

void renderBMP(BITMAPINFO * bmpinfo,const unsigned char * data)
{
	glDrawPixels(bmpinfo->bmiHeader.biWidth,bmpinfo->bmiHeader.biHeight,GL_RGB,GL_UNSIGNED_BYTE,data);
}

void renderBMP(int w,int h,const unsigned char * data)
{
	glDrawPixels(w,h,GL_RGB,GL_UNSIGNED_BYTE,data);
}

void drawPic(char *picFn,int w,int h)
{
	BITMAPINFO *bmpinfo;
	unsigned char * data;
	data=mLoadbmp("monkey.bmp",&bmpinfo);
	//renderBMP(bmpinfo,data);
	if(data)
		renderBMP(w,h,data);

}
void drawToTexture(GLuint tex,int Sx,int Sy,int Ex,int Ey,char *str)
{
	putStrScr(str,Ex-Sx,Ey-Sy);
	glFlush ();
	glBindTexture(GL_TEXTURE_2D,tex);
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0, Ex-Sx,Ey-Sy);
	//glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0, 256, 256);
}
/*
 *for draw to texture  
 */
void setProjectionTex(int w,int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0.0, w, 0.0, h, -1.0, 1.0);
}
void setProjectionScr(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0,1.0,-1.0,1.0,5.0,25.0);
}
void preDrawToTexture(GLuint tex,int Sx,int Sy,int Ex,int Ey)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	Tex_Win_w=Ex-Sx;
	Tex_Win_h=Ey-Sy;
	setProjectionTex(Ex-Sx,Ey-Sy);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
	cWH=getWH_Chars(Ex-Sx,Ey-Sy,3);
	//	printf("(%d,%d,%d,%d)\n",Sx,Sy,Ex,Ey);
	MoveTo2(Sx+20,Ey-20);
	setStartPos(Sx+20,Ey-20);
}
void afterDrawToTexture()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glViewport(0, 0, (GLsizei) Win_w, (GLsizei) Win_h);
}
void drawScrToTexture(GLuint tex,int Sx,int Sy,int Ex,int Ey)
{
	glFlush ();
	glBindTexture(GL_TEXTURE_2D,tex);
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0, Ex-Sx,Ey-Sy);
}
void drawToTexture2(GLuint tex,int Sx,int Sy,int Ex,int Ey,char *str)
{
	void initDisplay();
	initDisplay();
	preDrawToTexture(tex,Sx,Sy,Ex,Ey);
	//	putStrScr(str,Ex-Sx,Ey-Sy);
	//	putStrScr("i love you",Ex-Sx,Ey-Sy);
	/*
	   putStrScr("1\n");
	   putStrScr("2\n");
	   putStrScr("3\n");
	   drawInput(100,20,"i love you");
	   putStrScr("4\n");
	   putStrScr("\n");
	   */
	//putStrScr(str);

	static	htmlparser hp;
	static int first =1;
	if(first){
		hp.parsehtml(str);
		first =0;
	}
	hp.renderhtml();
	hp.printTree();
	//  drawInput2d(200,340,0,100,20);
	// drawButton2d(400,340,0,100,20);
	BITMAPINFO *bmpinfo;
	unsigned char * data;
	data=mLoadbmp("monkey.bmp",&bmpinfo);
	glRasterPos2f(200,20);
	renderBMP(bmpinfo,data);
	//    renderBMP(200,200,data);


	drawScrToTexture(tex,Sx,Sy,Ex,Ey);
	afterDrawToTexture();
}
void drawSquare5(double left,double right,double bottom,double top,double offset,int xyz)
{
	switch(xyz)
	{
		case 2://z=offset
			glBegin(GL_QUADS);
			//    glTexCoord2f (left,bottom);
			glVertex3f(left,bottom,offset);
			//    glTexCoord2f (right,bottom);
			glVertex3f(right,bottom,offset);
			//    glTexCoord2f (right,top);
			glVertex3f(right,top,offset);
			//    glTexCoord2f (left,top);
			glVertex3f(left,top,offset);
			glEnd();
			break;
		case 1://y=offset
			glBegin(GL_QUADS);
			//    glTexCoord2f (left,bottom);
			glVertex3f(left,offset,bottom);
			//    glTexCoord2f (right,bottom);
			glVertex3f(right,offset,bottom);
			//    glTexCoord2f (right,top);
			glVertex3f(right,offset,top);
			//    glTexCoord2f (left,top);
			glVertex3f(left,offset,top);
			glEnd();
			break;
		case 0://x=offset
			glBegin(GL_QUADS);
			//    glTexCoord2f (left,bottom);
			glVertex3f(offset,left,bottom);
			//    glTexCoord2f (right,bottom);
			glVertex3f(offset,right,bottom);
			//    glTexCoord2f (right,top);
			glVertex3f(offset,right,top);
			//    glTexCoord2f (left,top);
			glVertex3f(offset,left,top);
			glEnd();
			break;
	}
}
void drawCube6(double xleft,double xright,double ybottom,double ytop,double zfront,double zback)
{
	glNormal3f(0,-1,0);
	drawSquare5(xleft,xright,zback,zfront,ybottom,1);
	glNormal3f(0,1,0);
	drawSquare5(xleft,xright,zfront,zback,ytop,1);
	glNormal3f(-1,0,0);
	drawSquare5(ybottom,ytop,zfront,zback,xleft,0);
	glNormal3f(1,0,0);
	drawSquare5(ybottom,ytop,zback,zfront,xright,0);
	glNormal3f(0,0,1);
	drawSquare5(xleft,xright,ybottom,ytop,zfront,2);
	glNormal3f(0,0,-1);
	drawSquare5(xleft,xright,ytop,ybottom,zback,2);
}
void preRedraw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	setProjectionTex(Tex_Win_w,Tex_Win_h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	/*
	*/
}
void sufRedraw()
{
	afterDrawToTexture();
	/*
	   glMatrixMode(GL_PROJECTION);
	   glPopMatrix();
	   glMatrixMode(GL_MODELVIEW);
	   glPopMatrix();
	   */  
}
GLint getTexref()
{

}
int copyBackToFrt(int stx,int sty,int dw,int dh,int destx,int desty)
{
	MoveTo2(destx,desty);
	glCopyPixels(stx,sty, dw,dh,GL_COLOR);
}
int copyToCurTexture(int tx,int ty,int dsx,int dsy,int sw,int sh)
{
	glBindTexture(GL_TEXTURE_2D,texName1);
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,tx,ty,sw,sh, dsx,dsy);
}
/*
   void    printTex(GLubyte* tex1,int w,int h)
   {
   int i,j;
   for(i=0;i<w;i++)
   {
   for(j=0;j<h;j++)
   printf("(%d,%d,%d,%d)",tex1[i*w*4+j*4],tex1[i*w*4+j*4+1],tex1[i*w*4+j*4+2],tex1[i*w*4+j*4+3]);
   printf("\n");
   }
   }
   */

int  getColor()
{
	//return fl_color();
}
int setColor(int color)
{
	//int old = fl_color();
	//fl_color(color);
	//return old;
}
