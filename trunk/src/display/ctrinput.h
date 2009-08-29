#ifndef CTRINPUT_HEADER
#define CTRINPUT_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include "Widget.h"
class ctrInput: public Widget
{
    private:
        int maxc,curc;
        int startDrawPos;
        int curCursorPos;
        int curdx;
        char *pvalue;

    public:
        ctrInput();
        ctrInput(int w, int h);
        ctrInput(int x,int y, int z);
        ctrInput(int x,int y, int z,int sdx,int sdy,char* l);
        ctrInput(int x,int y, int z,char * s);
        ctrInput(int x,int y, int w,int h, char* l);
        ctrInput(int x,int y, int z, int w,int h, int d, char* l);

        ~ctrInput();
        void reset();
        char* getValue();
        void attachBuffer(char* buf){}
        int appandchar(char c);
        int deletechar(char c);
        int insertchar(char c);
        int backspacechar(char c);

    public:
        virtual void draw();
        virtual void reDraw();
        virtual int handleKey(int key){keyfunc( key, 0, 0);return 1;};
        void skeyfunc(int key,int x,int y);

    private:
        void initInput(int sdx,int sdy);
        void keyfunc(char  key,int x,int y);
    private:
        void init();
        void drawCursor();
        void moveCursor(int drt);
};
#endif
