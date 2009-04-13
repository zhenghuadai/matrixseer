#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/ScrollGroup.h>
#include <fltk/LightButton.h>
#include <fltk/Choice.h>
#include <fltk/Box.h>
#include <stdio.h>
#include <fltk/draw.h>
#include "html.h"
using namespace fltk;
class mainScroll :public ScrollGroup
{
public:
    mainScroll(int x,int y,int w,int h, const char*l=0, bool begin=false):ScrollGroup(x, y, w, h, l,begin){}
    htmlparser *hp;
    void draw();
};
