#ifndef __CSSBOX_H_
#define __CSSBOX_H_
#include <fltk/Box.h>
#include <fltk/Style.h>
#include <fltk/draw.h>
using namespace fltk;
typedef struct {
char width[4];
int color[4];

}cssBorderStyle;
#define SIZE 3
typedef struct LineStyle_{
int lineStyle_;
float lineWidth_;
const char * lineDashes_;
} LineStyle;
class CssBox : public Box {
public:
  void _draw(const Rectangle& r1) const;
  void inset(Rectangle& r) const {
    r.x(r.x()+1);
    r.y(r.y()+1);
    r.w(r.w()-(2+SIZE));
    r.h(r.h()-(2+SIZE));
  }
  bool is_frame() const {return true;}
  CssBox(const char* n) : Box(n) {}
};

extern Box*  CSS_BOX;
#endif
