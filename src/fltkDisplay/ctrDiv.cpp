#include "ctrDiv.h"
//#include <FL/fl_draw.H>
#if 0
void ctrDiv::draw() {
#ifdef DEBUG
    printf("%s : draw\n",label());
#endif
    Group::draw();
}
#endif
int ctrDiv::handle(int e) {
#ifdef DEBUG
    if (e != FL_MOVE) printf("%s : %s\n",label(),eventnames[e]);
#endif
#if 0
    switch(e)
    {
        case FL_ENTER:
          if(!pushed)
            labelcolor(FL_RED);
            fl_cursor(FL_CURSOR_HAND);
            redraw();
            break;
        case FL_LEAVE:
          if(!pushed)
            labelcolor(FL_BLUE);
            fl_cursor(FL_CURSOR_DEFAULT);
            redraw();
            break;
        case FL_FOCUS:
            break;
        case FL_PUSH:
            Fl::focus(this);
            pushed =1;
            break;
        case FL_KEYBOARD:
            if (e == FL_KEYBOARD && Fl::event_text()[0]) {
            }
            break;
    }
#endif
    if (Group::handle(e)) return 1;
    return 0;
}

