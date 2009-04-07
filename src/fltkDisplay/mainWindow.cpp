#include "mainWindow.h"
//#include <fltk/Fl_Menu_Item.H>
//#include <FL/fl_draw.H>
//#include <FL/Button.H>
#include "mdtime.h"
#include "ctrText.h"
void test_cb(Widget* w, void*) {
  Menu* mw = (Menu*)w;
}

void quit_cb(fltk::Widget*, void*) {
    exit(0);
}
#if 1
Fl_Menu_Item menutable[] = {
    {"&File",0,0,0,FL_SUBMENU},
    {"&Open",	ALT+'o', 0, 0, FL_MENU_INACTIVE},
    {"&Close",	0,	0},
    {"&Quit",	ALT+'q', quit_cb, 0, FL_MENU_DIVIDER},
    {0},
    {"&Edit",0,0,0,FL_SUBMENU},
    {"Undo",	ALT+'z',	0},
    {"Redo",	ALT+'r',	0, 0, FL_MENU_DIVIDER},
    {"Cut",	ALT+'x',	0},
    {"Copy",	ALT+'c',	0},
    {"Paste",	ALT+'v',	0},
    {"Inactive",ALT+'d',	0, 0, FL_MENU_INACTIVE},
    {"Clear",	0,	0, 0, FL_MENU_DIVIDER},
    {"Preferences",0,	0},
    {"Size",	0,	0},
    {0},
    {"View",CTRL + 'f',0,0,FL_SUBMENU},
    {"font Size",0,0,0}, 
    {"&Font",0,0,0,FL_SUBMENU /*, 0, FL_BOLD, 20*/},
    {"Normal",	0, 0, 0, 0, 0, 0, 14},
    {"Bold",	0, 0, 0, 0, 0, 0, 14},
    {"Italic",	0, 0, 0, 0, 0, 0, 14},
    {"BoldItalic",0,0,0, 0, 0, 0, 14},
    {"Small",	0, 0, 0, 0, 0, 0, 10},
    {"Emboss",	0, 0, 0, 0, 0 },
    {"Engrave",	0, 0, 0, 0, 0 },
    {"Shadow",	0, 0, 0, 0, 0 },
    {"@->",	0, 0, 0, 0, 0},
    {0},
    {0},
    {"History",CTRL + 'h',0,0,FL_SUBMENU},
    {"back",CTRL +'6',0,0},
    {"forward",CTRL +'8',0,0},
    {0},
    {"Favorite",CTRL +'j',0,0,FL_SUBMENU},
    {"clean",0},
    {0},
    {"Tool",0,0,0,FL_SUBMENU},
    {"download",0,0},
    {0},
    {"help",0,0,0,FL_SUBMENU},
    {"help",CTRL + 'h'},
    {0},
    {0}
};
#endif
Button *fb= 0;
mainWindow::mainWindow(int w,int h,const char *l):Window(w,h,l)
{
    begin();
       mainMenu = new MenuBar(0,0,w,40);
       mainMenu -> menu(menutable);
       mainMenu -> callback(test_cb);
       #if 1
       //scrollFrame = new ScrollGroup(0,40,w,h);
       scrollFrame = new mainScroll(0,40,w-20,h -100 );
       scrollFrame->hp = &hp;

       scrollFrame -> begin();
       scrollFrame->box(fltk::DOWN_BOX);
       scrollFrame->type(ScrollGroup::BOTH);
//       ctrText * bt  = new ctrText(0,0,100,100,"the button");
//       Widget * bt  = new Widget(50,50,100,100,"the button");
#endif
       //    urlInput = new Input(90,40,300,30,"Normal:");
       //    fb = new Button(5,40,30,30,"new"); 
};
void mainWindow::parseHtml()
{
}
void mainWindow::preDraw() {
    return ;
    extern void resetDisplay();
    void setdefaultfont(void);
    resetDisplay();
    setdefaultfont();
    hp.renderhtml();
}
void mainWindow::draw() {
    extern void resetDisplay();
#ifdef DEBUG
    printf("%s : draw\n",label());
#endif
    Window::draw();
    return ;
//    return ;
    resetDisplay();
    void setdefaultfont(void);
    setdefaultfont();
    mdtime(0);
    hp.renderhtml();
    printf("time:%f\n",mdtime(1));
    //    fl_line(100,100,200,100);
    //    fl_draw("text",100,100);
    //    fl_xyline(100,100,200,200,200);
    // if (key) fl_draw(&key, 1, cx, cy);
}
int mainWindow::handle(int e) {
#ifdef DEBUG
    if (e != FL_MOVE) printf("%s : %s\n",label(),eventnames[e]);
#endif
    if (crsr!=CURSOR_ARROW) {
        if (e == ENTER)
            cursor(crsr);
        if (e == LEAVE)
            cursor(CURSOR_ARROW);
    }
    if (Window::handle(e)) return 1;
    if (e == FOCUS) return 1;
    if (e == PUSH) {take_focus(); return 1;}
    /*
       if (e == FL_KEYBOARD && Fl::event_text()[0]) {
       key = Fl::event_text()[0];
       cx = Fl::event_x();
       cy = Fl::event_y();
       redraw();
       return 1;
       }
       */
    return 0;
}

