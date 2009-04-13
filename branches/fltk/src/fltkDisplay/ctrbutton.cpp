#include "ctrbutton.h"
//#include <FL/fl_draw.H>

void ctrButton::draw() {
#ifdef DEBUG
    printf("%s : draw\n",label());
#endif
    Button::draw();
}
int ctrButton::handle(int e) {
#ifdef DEBUG
    if (e != FL_MOVE) printf("%s : %s\n",label(),eventnames[e]);
#endif
    if (Button::handle(e)) return 1;
/*
if (e == FL_FOCUS) return 1;
    if (e == FL_PUSH) {Fl::focus(this); return 1;}
    if (e == FL_KEYBOARD && Fl::event_text()[0]) {
        redraw();
        return 1;
    }
    */
    return 0;
}

