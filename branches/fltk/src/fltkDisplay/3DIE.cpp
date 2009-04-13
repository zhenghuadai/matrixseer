//
// "$Id: 3DIE.cpp,v 1.2 2008-09-11 12:29:40 dzh Exp $"
//
// Menubar test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//
/*
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/fl_draw.H>
*/
#include <fltk/Window.h>
#include <fltk/Widget.h>
#include <fltk/run.h>
#include <stdio.h>
#include <stdlib.h>
//#include "flstring.h"
#include "string.h"
#include "mainWindow.h"
#include "dmfile.h"
#include "lib.h"
#include "jsapi.h"
#include "ctrinput.h"
#include "ctrbutton.h"
#include "ctrText.h"
#include "ctrP.h"
int Win_w = 1280;
int Win_h =1024;
int mDC_w = Win_w - 40;
int mDC_h = Win_h - 40;
using namespace fltk;
//Fl_Menu_* menus[4];
//Fl_Input * input[2];
#if 1
extern void setStartPos(int x,int y);
extern void resetDisplay();
extern void initDisplay();
ctrInput *pctrInputs[100];
int pctrIindex = 0;
ctrButton * pctrButtons[100];
int pctrBindex = 0;
char * httpcont = 0;
char * url= 0;
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

void init(int argc,char **argv)
{
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
        printf("result:%s\n",s);
    }
}
#endif
int main(int argc, char **argv) 
{
#if 1
    //Fl::set_color(Fl_Color(15),0,0,128);
    //Fl_Window window(WIDTH,Win_h);
    int ret;
    ret=     initJS();
    if(ret){
        printf("error");
        exit(0);
    }
    int i;
    extern int getCharWidth(char c);
    extern void setdefaultfont(void);

    setdefaultfont();
#if PDEB 
    for(i =0;i<256;i++)
    {
        printf("[%d %d] ",i,getCharWidth((char)i));
        if(i % 16 ==0)printf("\n");
    }
#endif
    testJS();
    init(argc,argv);

    //setStartPos(0,100);
    initDisplay();
    resetDisplay();
#endif
    mainWindow  window(Win_w,Win_h,(const char *)0);
    //Window  window(Win_w,Win_h);
    //  window.begin();
    window.hp.parsehtml(httpcont);
//    ctrP *cP = new ctrP(0,200,200,200,"ctrP");
    //window.preDraw();
    //window.size_range(100,400,0,1000);
    window.scrollFrame->end();
    window.end();
    //window.show(argc, argv);
    window.show();
    return run();
}

