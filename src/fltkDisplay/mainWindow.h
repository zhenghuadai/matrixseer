#ifndef MAINWINDOW_HEADER
#define MAINWINDOW_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include <stdlib.h>
#include <stdio.h>
/*
#include <FL/Fl.H>
#include <FL/Window.H>
#include <FL/Toggle_Button.H>
#include <FL/Menu_Button.H>
#include <FL/Box.H>
#include <FL/Button.H>
#include <FL/Input.H>
#include <FL/Menu_Bar.H>
*/
#include <fltk/run.h>
//#include <fltk/Fl_Menu_Item.H>
#include <fltk/compat/FL/Fl_Menu_Item.H>
#include <fltk/Item.h>
#include <fltk/ItemGroup.h>
#include <fltk/Divider.h>
#include <fltk/Window.h>
#include <fltk/Box.h>
#include <fltk/MenuBar.h>
#include <fltk/PopupMenu.h>
#include <fltk/InputBrowser.h>
#include <fltk/Choice.h>
#include <fltk/CycleButton.h>
#include <fltk/Button.h>
#include <fltk/draw.h>
#include <string.h>
#include <fltk/Slider.h>
#include <fltk/StringList.h>
#include <fltk/events.h>
#include <fltk/ask.h>
#include <fltk/Cursor.h>
#include "html.h"
#include <fltk/Output.h>
#include <fltk/ToggleButton.h>
#include <fltk/PopupMenu.h>
#include <fltk/Choice.h>
#include <fltk/Tooltip.h>
#include <fltk/ScrollGroup.h>
#include "ctrScroll.h"
using namespace fltk;
class mainWindow : public Window {
    int handle(int);
    void draw();
    int cx, cy; char key;
    Cursor *crsr;

    Input * urlInput,*searchInput;
    MenuBar * mainMenu;
    public:
//    ScrollGroup *scrollFrame;
    mainScroll *scrollFrame;
    mainWindow():Window(0,0,""){};
    mainWindow(int x,int y,const char *l=0);//:Window(x,y,l){};
    mainWindow(Box* b,int x,int y,const char *l)
        : Window(x,y,l), crsr(CURSOR_ARROW) {box(b); key = 0;};

    mainWindow(Box* b,int x,int y,int w,int h,const char *l)
        : Window(x,y,w,h,l) {box(b); key = 0;}

    void use_cursor(Cursor *c) { crsr = c; }
    void parseHtml();
    void preDraw();
     htmlparser hp;
};

#endif
