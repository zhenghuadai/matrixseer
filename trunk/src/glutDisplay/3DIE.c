#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lib.h"
#include "display.h"
#include "glutbitmap.h"
#include "app.h"
#include "jsapi.h"
#include "string"
#include "dmfile.h"
#include "font.h"
#include "debprintf.h"
using namespace std;
int getWinh();
char * httpcont=0;
int Win_w=1024;
int Win_h=768;
static int W_CHARS=200;
static int H_CHARS=200;
static GLubyte *httptex1;
GLuint texName1;
static int DisplayMode = PLAINMODE;
float globalColor[4]={1.0,1.0,1.0,1.0};
float globalColor2[4]={.0,.0,1.0,1.0};
//ctrInput *pctrInput0=0;
//ctrInput * pctrInputs[10];
Widget *pctrInput0=0;
Widget * pctrInputs[10];
ctrButton *  pctrButtons[10];
int pctrIindex = 0;
int pctrBindex =0;

Widget* curDoc=0;
Widget* onFocusWidget=0;

void MtrxArc(float zoff);
//extern const BitmapFontRec   MglutBitmapTimesRoman10;//MglutBitmapTimesRoman10;
//#include "glut_tr10.c"
//void * MGLUT_BITMAP_TIMES_ROMAN_10=(void *)&MglutBitmapTimesRoman10;//MglutBitmapTimesRoman10;

#define turnOnLight0() { \
    glEnable(GL_LIGHTING);\
    glEnable(GL_LIGHT0);\
}

#define TEXW   2048 
#define TEXH  1024 
#define RW Win_w//*5/6
#define RH Win_h//*5/6
#define DRW (double)RW
#define DRH (double)RH
#define RA 1
static int ra=0;
static float eyeZ = 5.0;
static void inittex()
{
    int i;
#ifdef HTTPTEX
    httptex1=(GLubyte *)malloc(TEXW * TEXH * 4 * sizeof(GLubyte));
    for(i=0;i<TEXW * TEXH ;i+=4)
    {
        httptex1[i]=(GLubyte)225;
        httptex1[i+1]=(GLubyte)0;
        httptex1[i+2]=(GLubyte)0;
        httptex1[i+3]=(GLubyte)255;
    }
#endif
    glGenTextures(1, &texName1);
    glBindTexture(GL_TEXTURE_2D, texName1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXW , TEXH,
            0, GL_RGBA, GL_UNSIGNED_BYTE,NULL/* httptex1*/);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

void setLight0()
{
    GLfloat l_pos[]={0.0,.0,5.0, 0.0};
    GLfloat l_color[]={1.0,1,1,1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,l_pos);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,l_color);
    GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat position[] = {0.0, 3.0, 3.0, 0.0};

    GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat local_view[] = {0.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

    glFrontFace(GL_CW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST); 

}


void initGL()
{
    turnOnLight0();
    setLight0();

}

void init(int argc,char * argv[])
{
    char * url;
    initGL();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    inittex();
    if(argc ==1) 
        url = "www.google.com";
    else
        url = argv[1];
    if(isfile(url)){
        httpcont = getsfromfile(url);
        if(httpcont == 0) {
            exit(0);
        }
    }
    else
        httpcont=httpsock(url,(int*)NULL);
    if(httpcont == NULL) 
        httpcont = getsfromfile("err.html");
}
void setProjectionP()
{
}

/*
 * Move object into position.  Use 3rd through 12th 
 * parameters to specify the material property.  Draw a teapot.
 */
void renderTeapot(GLfloat x, GLfloat y, GLfloat z,
        GLfloat ambr, GLfloat ambg, GLfloat ambb,
        GLfloat difr, GLfloat difg, GLfloat difb,
        GLfloat specr, GLfloat specg, GLfloat specb, GLfloat shine)
{
    GLfloat mat[4];

    glPushMatrix();
    glTranslatef(x, y, z);
    //   glRotatef(spin, 0.0, 0.0, 1.0);
#if 0 
    mat[0] = ambr; mat[1] = ambg; mat[2] = ambb; mat[3] = 1.0;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
    mat[0] = difr; mat[1] = difg; mat[2] = difb;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
    mat[0] = specr; mat[1] = specg; mat[2] = specb;
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
    glMaterialf(GL_FRONT, GL_SHININESS, shine * 128.0);
#endif
    //   glCallList(teapotList);
    glDisable(GL_TEXTURE_2D);
    glutSolidTeapot(.2);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}


#define LX (-1) 
#define BY (-1) 
#define RX 1 
#define TY 1
#define LX2 ( LX) 
#define BY2 ( BY) 
#define RX2 ( RX) 
#define TY2 ( TY)
#define ZF 0
#define ZB -6 

void MtrxPlane(float zoff)
{
    glBegin(GL_QUADS);
#if 1 
    glTexCoord2f(0.0, 0.0);    		   glVertex3f(LX2, BY2, zoff);
    glTexCoord2f( DRW / TEXW, 0.0);    glVertex3f(RX2, BY2, zoff);
    glTexCoord2f(DRW / TEXW,DRH/TEXH); glVertex3f(RX2, TY2, zoff);
    glTexCoord2f(.0, DRH/TEXH);        glVertex3f(LX2, TY2, zoff);
#else
    glTexCoord2i(0, 0);      		   glVertex3f(LX2, BY2, zoff);
    glTexCoord2i(2, 0);         	   glVertex3f(RX2, BY2, zoff);
    glTexCoord2i(2,2);                 glVertex3f(RX2, TY2, zoff);
    glTexCoord2i(0, 2);       	       glVertex3f(LX2, TY2, zoff);
#endif
    glEnd();

}
void MtrxCubic()
{
    // glRotatef(ra,1,0,0);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);     		  glVertex3f(LX, BY, ZF);
    glTexCoord2f( DRW / TEXW, 0.0);   	  glVertex3f(LX2, BY2, ZB);
    glTexCoord2f(DRW / TEXW,DRH/TEXH);    glVertex3f(LX2, TY2, ZB);
    glTexCoord2f(.0, DRH/TEXH);       	  glVertex3f(LX, TY, ZF);

    glTexCoord2f(0.0, 0.0);    		      glVertex3f(LX2, BY2, ZB);
    glTexCoord2f( DRW / TEXW, 0.0);       glVertex3f(RX2, BY2, ZB);
    glTexCoord2f(DRW / TEXW,DRH/TEXH);    glVertex3f(RX2, TY2, ZB);
    glTexCoord2f(.0, DRH/TEXH);       	  glVertex3f(LX2, TY2, ZB);

    glTexCoord2f(0.0, 0.0);    		      glVertex3f(RX2, BY2, ZB);
    glTexCoord2f( DRW / TEXW, 0.0);       glVertex3f(RX, BY, ZF);
    glTexCoord2f(DRW / TEXW,DRH/TEXH);    glVertex3f(RX, TY, ZF);
    glTexCoord2f(.0, DRH/TEXH);       	  glVertex3f(RX2, TY2, ZB);

    glEnd();
}

void MtrxCubicArc()
{
    // glRotatef(ra,1,0,0);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);    		   glVertex3f(LX, BY, ZF);
    glTexCoord2f( DRW / TEXW, 0.0);    glVertex3f(LX2, BY2, ZB);
    glTexCoord2f(DRW / TEXW,DRH/TEXH); glVertex3f(LX2, TY2, ZB);
    glTexCoord2f(.0, DRH/TEXH);        glVertex3f(LX, TY, ZF);

    glTexCoord2f(0.0, 0.0);    		   glVertex3f(RX2, BY2, ZB);
    glTexCoord2f( DRW / TEXW, 0.0);    glVertex3f(RX, BY, ZF);
    glTexCoord2f(DRW / TEXW,DRH/TEXH); glVertex3f(RX, TY, ZF);
    glTexCoord2f(.0, DRH/TEXH);        glVertex3f(RX2, TY2, ZB);

    glEnd();
    MtrxArc(ZB);
}

void MtrxArc(float zoff)
{
    int i;
    static 	float arc[22]={-1.000000,0.000000,-0.813473,-0.380160,-0.618034,-0.680249,
        -0.415823,-0.896978,-0.209057,-1.027972,-0.000000,-1.071797,
        0.209057,-1.027972,0.415823,-0.896978,0.618034,-0.680249,
        0.813473,-0.380160,1.000000,0.000000};
    float stepw =DRW/TEXW;
    float h =DRH/TEXH;
    float w=.0;
    stepw=stepw/10;
    for(i=0;i<10;i++)
    {
        glBegin(GL_QUADS);
        glTexCoord2f(w,0.0);		glVertex3f(arc[2*i],BY,arc[2*i+1]+zoff);
        glTexCoord2f(w+stepw,0.0); 	glVertex3f(arc[2*(i+1)],BY,arc[2*(i+1)+1]+zoff);
        glTexCoord2f(w+stepw,h);	glVertex3f(arc[2*(i+1)],TY,arc[2*(i+1)+1]+zoff);
        glTexCoord2f(w,h);		glVertex3f(arc[2*i],TY,arc[2*i+1]+zoff);
        w+=stepw;
        glEnd();
    }
    //    glutSolidTeapot(0.2);
}

void display(void)
{
    initGL();
    glColor4fv(globalColor);
    /*
    //glReadPixels(0,0,TEXW,TEXH,GL_RGBA,GL_UNSIGNED_BYTE,httptex1);
    //glTexSubImage2D(GL_TEXTURE_2D,0,0,0,TEXW,TEXH,GL_RGBA,GL_UNSIGNED_BYTE,httptex1);
    // glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0, TEXW, TEXH);
    //printTex(httptex1,TEXW,TEXH);
    */
    glClearColor(1.0,1.0,.0,.0); 
    glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    switch ( DisplayMode){
        case CUBICARCMODE:
            MtrxCubicArc();
            break;
        case CUBICMODE:
            MtrxCubic();
            break;
        case PLAINMODE:
            MtrxPlane(0.0);
            break;
        case ARCMODE:
            MtrxArc(0.0);
            break;
    }
    renderTeapot(0., -.6,  -1.0, 0.05, 0.0, 0.0, 0.5, 0.4, 0.4,
            0.7, 0.04, 0.04, .078125);
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();

}

void reshape(int w, int h)
{
    Win_w=w;
    Win_h=h;
    //getWH_Chars(Win_w, h,1);
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho (0.0, w, 0.0, h, -100.0, 100.0);
    //   gluPerspective(120.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
    glFrustum(LX,RX,BY,TY,5.0,25.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //	glTranslatef(0.0, 0.0, -3.6);
    gluLookAt((RX+LX)/2.0, (TY+BY)/2.0, eyeZ,
            (RX+LX)/2.0, (TY+BY)/2.0, -100.0,
            .0,1.0,.0);
    glColor4fv(globalColor2);
    drawToTexture2(texName1,0,0,RW,RH,httpcont);
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard(unsigned char key, int x, int y)
{
    static int i=0;
    if((key < 127)&&(key >= 0)) {
        if((  onFocusWidget != NULL)&&(onFocusWidget != curDoc)){ 
            onFocusWidget ->handleKey(key);
            onFocusWidget -> redraw();
            return ;
        }
    }
    debprintf("%0x %0x\n", onFocusWidget, pctrInput0);
    switch (key) {
        case 27:
            exit(0);
        case 13:
            break;
        case 'e': 
            //	ra=1;
            //	if(ra>360)ra=0;
            glRotatef(RA,0,1,0);
            //glutPostRedisplay();
            break;
        case 'q':
            glRotatef(-RA,0,1,0);
            //glutPostRedisplay();

            break;
        case 'd': 
            glTranslatef(.1,0,0);
            //glutPostRedisplay();
            break;
        case 'a': 
            glTranslatef(-.1,0,0);
            //glutPostRedisplay();
            break;
        case 'w': 
            //	eyeZ += 0.01;
            glTranslatef(0,0,.1);
            //  gluLookt(.0,.0,5.0,.0,.0,-100.0,.0,1.0,.0);
            //glutPostRedisplay();
            break;
        case 's': 
            //	eyeZ -= .01;
            //gluLookAt(.0,.0,5.0,.0,.0,-100.0,.0,1.0,.0);
            glTranslatef(0,0,-.1);
            //glutPostRedisplay();
            break;
        case 'j':
            DisplayMode++;
            DisplayMode %= 4;
            //glutPostRedisplay();
            break;
        default:
            //	url[i++]=key;
            //glutPostRedisplay();
            break;
    }
    glutPostRedisplay();
}
void ctrlkey(int key,int x,int y)
{
    if((pctrInput0)&&(pctrInput0 -> onFocus())){
        //pctrInput0->skeyfunc(key,x,y);
        pctrInput0 -> redraw();
    }
    switch (key){
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
            break;
        default:
            break;
    }
    glutPostRedisplay();
}
void mouse(int button, int state, int x, int y) 
{
    int z = 0;
    y = getWinh() - y;
    if(curDoc)
        if( curDoc -> hitMe(x, y, z)) 
            onFocusWidget = curDoc -> getObjOnFocus(x, y, z);
        else 
            onFocusWidget = curDoc;
    debprintf("[%d %d]hit  %0x\n", x,y, onFocusWidget);
    switch ( button)
    {
        case GLUT_LEFT_BUTTON:
            switch ( state) {
                case GLUT_DOWN:
                    onFocusWidget->handleButton( ButtonEvent( 1, x, y));
                    break;
                case GLUT_UP:
                    onFocusWidget->redraw();

                    break;
            }
            break;
        case GLUT_RIGHT_BUTTON:
            break;
    }
    glutPostRedisplay();
}
JSRuntime *rt;
JSContext *cx;
JSObject *globalObj;
#include "JSDocument.h"
JSClass globalClass =
{
    "Global", 0,
    JS_PropertyStub,  JS_PropertyStub,JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub,  JS_FinalizeStub
};
int initJS()
{
    rt = JS_Init(1000000L);
    if ( !rt) 
        return 1;
    cx = JS_NewContext(rt, 8192);
    if ( !cx )
        return 1;
    globalObj = JS_NewObject(cx, &globalClass, 0, 0);
    if ( !globalObj )
        return 1;

    JS_InitStandardClasses(cx, globalObj);
    JSDocument::JSInit(cx, globalObj);
    return 0;
}
int destroyJS()
{
    JS_DestroyContext(cx);
    JS_Finish(rt);
    return 0;
}
int testJS()
{
    jsval retval;
    JSString *str;
    char *myscript0 = "var c = new Customer();\
                       c.name = \"Franky\";\
                       c.age = 32;\
                       c.computeReduction();";
    char * myscript = " var document = new Document(); ";  
    uintN lineno=0;
    JSBool ok ;
    ok= JS_EvaluateScript(cx,globalObj,myscript,strlen(myscript),"abc",lineno,&retval);
    if(ok == JS_TRUE)
    {
        str = JS_ValueToString(cx,retval);
        char *s =JS_GetStringBytes(str);
        debprintf("result:%s\n",s);
    }
}
int main(int argc, char** argv)
{
    int ret;
    ret =   initJS();
    if(ret){
        debprintf("error");
        exit(0);
    }
    testJS();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize(Win_w, Win_h);
    glutInitWindowPosition (0, 0);
    glutCreateWindow("Matrix");
    init(argc,argv);
    defaultfont();
    //    selectfontfromid(MGLUT_BITMAP_TIMES_ROMAN_24);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutSpecialFunc(ctrlkey);
    glutMainLoop();
    destroyJS();
    return 0;
}


int getWinw()
{
    return Win_w;
}

int getWinh()
{
    return Win_h;
}
void setFocusInput(Widget* ci)
{
    pctrInputs[0]=pctrInput0=ci;
    ci->getFocus();
}

void setCurDocument(Widget* o)
{
    curDoc = o;
}
