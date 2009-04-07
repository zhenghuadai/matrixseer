//
// "$Id: hello.cxx,v 1.1.1.1 2007-11-30 13:27:32 dzh Exp $"
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

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <stdio.h>
#include <stdlib.h>
//#include "flstring.h"
#include <FL/fl_draw.H>
#include "string.h"
int Win_w = 1280;
int Win_h =1024;
int mDC_w = Win_w - 40;
int mDC_h = Win_h - 40;
Fl_Menu_* menus[4];
Fl_Input * input[2];
int pctrIindex = 0;
int pctrBindex = 0;
char * httpcont = 0;
char * url= 0;

int initJS()
{
    return 0;
}
int destroyJS()
{
    return 0;
}

void init(int argc,char **argv)
{
}
int testJS()
{
}

int main(int argc, char **argv) {
    //Fl::set_color(Fl_Color(15),0,0,128);
    //Fl_Window window(WIDTH,Win_h);
   int ret;
    //mainWindow  window(Win_w,Win_h,(const char *)0);
    Fl_Window  window(Win_w,Win_h,(const char *)0);
  //  window.hp.parsehtml(httpcont);

    //window.size_range(100,400,0,1000);
    window.end();
    window.show(argc, argv);
    return Fl::run();
}

