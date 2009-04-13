#include "ctrP.h"
//#include <FL/fl_draw.H>
#include "display.h"
void ctrP::draw() {
#ifdef DEBUG
    printf("%s : draw\n",label());
#endif
    Group::draw();
}


void ctrP::StartR(){
    getCurrentRasterPos(pos);
    if(PostionFlag == FIXEDPOS){
        pushRect();
        MoveTo2(x() + FirstLineIndent,y());
        setRect(x(),y(),w(),h());
        return ;
    }
#ifdef PDEB
    printf("[%d %d] ->", pos[0],pos[1]);
#endif
    position(pos[0],pos[1]);
    resize(pos[0],pos[1],w(),h());
#ifdef PDEB
    printf("ctrP [%d %d %d %d ] ->", x(),y(),w(),h());
#endif
    MoveTo2(pos[0]+FirstLineIndent,pos[1]);
}
void ctrP::EndR(){
    if(PostionFlag == FIXEDPOS){
        MoveTo2(pos[0],pos[1]);
        popRect();
        return ;
    }
    MoveTo2(x()+w(),y()+h());
    //   end();
#ifdef PDEB
    getCurrentRasterPos(pos);
    printf("[%d %d]\n", pos[0],pos[1]);
#endif

}

int ctrP::handle(int e) {
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

