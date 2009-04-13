#include "ctrA.h"
#include <fltk/events.h>
//#include <FL/fl_draw.H>

void ctrA::draw() {
#ifdef DEBUG
    printf("%s : draw\n",label());
#endif
    Widget::draw();
}
int ctrA::handle(int e) {
#ifdef DEBUG
    if (e != MOVE) printf("%s : %s\n",label(),eventnames[e]);
#endif
    switch(e)
    {
        case ENTER:
          if(!pushed)
            labelcolor(RED);
            cursor(CURSOR_HAND);
            redraw_label();
            redraw();
            break;
        case LEAVE:
          if(!pushed)
            labelcolor(BLUE);
            cursor(CURSOR_DEFAULT);
            redraw();
            break;
        case FOCUS:
            break;
        case PUSH:
            take_focus();
            labelcolor(GREEN);
            pushed =1;
            redraw();
            break;
        case KEY:
            if (e == KEY && event_text()[0]) {
            }
            break;
    }

    if (Widget::handle(e)) return 1;
    return 0;
}

