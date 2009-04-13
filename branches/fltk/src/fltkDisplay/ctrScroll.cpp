#include "ctrScroll.h"
void mainScroll::draw(){
    extern void resetDisplay();
    void setdefaultfont(void);
    ScrollGroup::draw();
    resetDisplay();
    setdefaultfont();
    return;
    hp->renderhtml();
}
