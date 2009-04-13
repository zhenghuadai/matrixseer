#include <stdio.h>
#include "html.h"
#include "stack.h"
#include "string.h"
#ifndef __HTMLMAIN  
#include "ctrinput.h"
#include "ctrA.h"
#include "ctrP.h"
#include "ctrDiv.h"
#include "app.h"
#include "font.h"
#include "display.h"
#define UPY 1
#define DOWNY -1
#define FORWDY UPY
//#include "jsglobal.h"
//#include "Document.h"
int putStrScr(char *content);
void drawInput(int dx,int dy);
void MoveToxy(int x,int y);
void goDown(int);
void drawButton(int dx,int dy,char * s);
void drawFrame(double x,double y,double z,double dx,double dy);
void drawLine(double x,double y,double z,double x2,double y2,double z2);
void drawFrame(int x,int y,int z,int dx,int dy);
void drawLine(int x,int y,int z,int x2,int y2,int z2);
void setStartPos(int x,int y);
void moveToNextLine();
void moveToNextLine(int dy);
void pushStartPos();
void popStartPos();
int  setColor(int);
void  mrender(char *f,char *s)
{
	putStrScr(s);
}
void  mrender(char *s)
{
	putStrScr(s);
}
char *tagfuncprintf(char * format,char *s)
{
	if((format[0]=='%')&&(format[1]=='s'))
		putStrScr(s);
	else{

	}
	return s;
}
#else
#define mrender(format,...)  printf(format,##__VA_ARGS__)
#endif
#undef OP
//#define OP(a) int htmlparser::a(char *htdoc,int which) 
#define OP(a) int htmlparser::a(pHtmlNode htnode)
#define GetArg char * htdoc; htdoc=htnode->value;
OP(Sscriptfunc)
{
	GetArg
		char *ppos=htdoc;
	{
		//  if(htdoc)  printf("%s",htdoc);
		return STARTTAG;
	}
}
OP(Escriptfunc)
{
	GetArg
		char *ppos=htdoc;
	//_3dhpop();
	//	printf("[     VALUE:%s ]",htdoc);

	return ENDTAG;
}
OP(Evaluefunc)
{
	GetArg
		//mrender("%s",htdoc);
}
OP(Sheadfunc)
{
	return STARTTAG;
}
OP(Eheadfunc)
{
	return ENDTAG;
}
OP( Sbodyfunc)
{
	GetArg
		mrender("%s",htdoc);

	return STARTTAG;
}
OP( Ebodyfunc)
{
	//_3dhpop();
	return ENDTAG;
}
OP(Spfunc)
{
    if((htnode -> obj)){	
        ctrP *mpctrP = (ctrP*)(htnode ->obj);
        moveToNextLine();
    //    mpctrP -> StartR();
        mpctrP -> show();
      //  mpctrP -> redraw();
        setColor(GM_BLACK);
    }
    return ENDTAG;
}
OP( Epfunc)
{
    if((htnode -> obj)){	
        ctrP *mpctrP = (ctrP*)(htnode ->obj);
      //  mpctrP -> EndR();
    }
    return ENDTAG;
}
OP( Shtmlfunc)
{
    GetArg
        char *ppos=htdoc;
    {
        mrender("%s",htdoc);
    }
    return ENDTAG;
}
OP( Ehtmlfunc)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
    return ENDTAG;
}
OP( Stitlefunc)
{
    GetArg
        char *ppos=htdoc;
    {
        mrender("%s",htdoc);
    }
    return ENDTAG;
}
OP( Etitlefunc)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
    return ENDTAG;
}
#define DOWND_ + 
OP(Stablefunc)
{
    GetArg
        char *ppos=htdoc;
    int tablePos[4];
    tableattr_t *table = ( tableattr_t *)getAttrBaseP(htnode);
    gridPos_t *src = table -> colsW;
    getCurrentRasterPos(tablePos);
    table -> startx = tablePos[0];
    table -> starty = tablePos[1];
    //printf("[%d %d %d ]\n", tablePos[0],tablePos[1],tablePos[2]);
    int i;
    for(i=0;i< table -> cols;i++){
        //    printf("[%d %d]  ",src[i].offset,src[i].length);
        src[i].realOffset =src[i].offset + tablePos[0];
    }
    printf("\n");
    gridPos_t *rowsH = table -> rowsH;
    for(i=0;i< table -> rows;i++){
        //  printf("[%d %d]  ",rowsH[i].offset,rowsH[i].length);
        rowsH[i].realOffset =tablePos[1] DOWND_ rowsH[i].offset;
    }
    //   printf("\n");
    drawFrame(tablePos[0],tablePos[1],tablePos[2],table -> w, DOWND_  (table ->h));
    for(i =1;i< table -> cols;i++){
        drawLine(src[i].realOffset,tablePos[1],tablePos[2],src[i].realOffset,tablePos[1] DOWND_ (table ->h),tablePos[2]);
    }
    for(i =1;i< table -> rows;i++){
        drawLine(tablePos[0],rowsH[i].realOffset,tablePos[2],tablePos[0] + table->w,rowsH[i].realOffset,tablePos[2]);
    }
    return ENDTAG;
}
OP( Etablefunc)
{
    tableattr_t *table = ( tableattr_t *)( getAttrBaseP(htnode));

    MoveToxy(table ->startx,table -> starty DOWND_ table -> h DOWND_ 25); 
    //_3dhpop();
    return ENDTAG;
}
OP(Strfunc)
{
    {
    }

    {
    }
    return ENDTAG;
}
OP( Etrfunc)
{
    GetArg
        char *ppos=htdoc;
    mrender("\n");
    //_3dhpop();
    return ENDTAG;
}

OP(Stdfunc)
{
    GetArg;
    pushStartPos();
    tdattr_t *tdattr = (tdattr_t* ) (getAttrBaseP(htnode));
    //    printf("[%d %d] ",*(tdattr -> pstartx),*(tdattr -> pstarty));
#if FORWDY == DOWNY
    MoveToxy(*(tdattr -> pstartx),*(tdattr -> pstarty) DOWND_ 20);
    setStartPos(*(tdattr -> pstartx),*(tdattr -> pstarty) DOWND_ 20);
#else
    MoveToxy(*(tdattr -> pstartx),*(tdattr -> pstarty) );
    setStartPos(*(tdattr -> pstartx),*(tdattr -> pstarty) );
#endif
    {
        char *ppos=htdoc;
        if(ppos !=NULL){
            mrender("%s",ppos);
            /*mrender(" ");
              char buf[1024];
              memset(buf,' ',2 * (*(tdattr -> colsMaxW) - tdattr -> w));
              buf[2* (*(tdattr -> colsMaxW) - tdattr -> w)] = 0;
              mrender(buf);
              */
        }
        //    printf("maxw %d \n",*(tdattr -> cellMaxW));
    }

    {
    }
    return ENDTAG;
}

OP( Etdfunc)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
    popStartPos();
    return ENDTAG;
}

OP(Sulfunc)
{
    GetArg
        char *ppos=htdoc;
    {
        mrender("*");
    }

    {
    }
    return ENDTAG;
}
OP( Eulfunc)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
    mrender("\n");
    return ENDTAG;
}
OP(Slifunc)
{
    GetArg
    {
        char *ppos=htdoc;
        mrender("^");
        mrender("%s",ppos);
    }

    {
    }
    return ENDTAG;
}
OP( Elifunc)
{
    GetArg
        char *ppos=htdoc;
    mrender("\n");
    //_3dhpop();
    return ENDTAG;
}
OP(imgfunc)
{
    void drawPic(char *picFn,int w,int h)    ;
    void forward0(int x,int y);
    GetArg;
    char *ppos=htdoc;
#if 0 // DOWND_ == -
    goDown(80);
    drawPic("monkey.bmp",100,100);
    forward0(100,80);
#else
    imgattr_t *pImg= (imgattr_t*)htnode ->attr;
    drawPic("monkey.bmp",pImg->w,pImg->h);
    forward0(pImg->w,0);
#endif
    return ENDTAG;
}
OP(Sh1func)
{
    GetArg
        char *ppos=htdoc;
    {
        mrender("%s",ppos);
    }

    {
    }
    return ENDTAG;
}
OP( Eh1func)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
    return ENDTAG;
}
OP(Sh2func)
{
    GetArg
    {
        char *ppos=htdoc;
        mrender("%s",ppos);
    }

    {
    }
    return ENDTAG;
}
OP( Eh2func)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
    return ENDTAG;
}
OP(Sh3func)
{
    GetArg
    {
        char *ppos=htdoc;
        mrender("%s",ppos);
    }

    {
    }
    return ENDTAG;
}
OP( Eh3func)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
    return ENDTAG;
}
OP(Sh4func)
{
    GetArg
        char *ppos=htdoc;
    {
        mrender("%s",ppos);
    }

    {
    }
    return ENDTAG;
}
OP( Eh4func)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
    return ENDTAG;
}
OP(Sh5func)
{
    GetArg
    {
        char *ppos=htdoc;
        mrender("%s",ppos);
    }

    {
    }
    return ENDTAG;
}
OP( Eh5func)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
    return ENDTAG;
}
OP(metafunc)
{
    return ENDTAG;
}
OP(linkfunc)
{
    GetArg
        char *ppos=htdoc;
    return ENDTAG;
}
OP(Sgeneralfunc)
{
    GetArg
    {
        char *ppos=htdoc;
        mrender("%s",ppos);
    }
    return ENDTAG;
}
OP( Egeneralfunc)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
}
OP(SgetErrtag)
{
    return -1;
}
OP(Sstylefunc)
{
    return ENDTAG;
}
OP( Estylefunc)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
}
OP(Safunc)
{
    if((htnode -> obj)){	
        ctrA *mpctrA = (ctrA*)(htnode ->obj);
        mpctrA -> setxyz();
    //    mpctrA -> active();
    //    mpctrA -> show();
        mpctrA -> redraw();
        setColor(GM_BLACK);
    }
    return ENDTAG;
#ifdef __HTMLMAIN 
    GetArg
        char *ppos=htdoc;
    int old = setColor(216);
    mrender("\033[33m%s\033[0m",ppos);
    setColor(old);
    return ENDTAG;
#endif
}
OP( Eafunc)
{
    GetArg
        char *ppos=htdoc;
    //_3dhpop();
}
OP(inputfunc)
{
    //char * pattr=(char *)getAttrBaseP(htnode).value;
    inputattr_t *p_inputattr =(inputattr_t*) (getAttrBaseP(htnode));
#ifndef __HTMLMAIN 
    switch(p_inputattr -> type)
    {
        case INPUTINPUT:
            //if( strstr(pattr,"type") == NULL) 
            {
                //	ctrInput ctrInput1(300,22);
                //	ctrInput1.draw();//(300,20);
                //	if(! pctrInput0) {
                if((htnode -> obj)){
                    ctrInput * mpctrInput=(ctrInput*)(htnode -> obj);
                    mpctrInput -> setxyz();
                    mpctrInput -> show();
                    mpctrInput -> redraw();
                }
            }
            break;
                case INPUTBUTTON:

            //if(strstr(pattr,"submit") != NULL)
            {
                if((htnode -> obj)){	
                    ctrButton *mpctrButton = (ctrButton*)(htnode ->obj);
                    mpctrButton -> setxyz();
                    mpctrButton -> show();
                    mpctrButton -> redraw();
                }
            }
            break;
            }
#endif
    }

    OP( brfunc)
    {
        if(htnode -> value) 
            moveToNextLine((int) htnode -> value );
        else
            moveToNextLine();

        //mrender("\n");
        return ENDTAG;
    }
    OP(Sdivfunc)
    {
        if((htnode -> obj)){	
            ctrDiv *mpctrDiv = (ctrDiv*)(htnode ->obj);
            MoveToxy(mpctrDiv -> x(), mpctrDiv -> y());
            //    mpctrP -> begin();
            mpctrDiv -> show();
            mpctrDiv -> redraw();
            setColor(GM_BLACK);
            pushRect();
            setRect(mpctrDiv -> x(),mpctrDiv -> y(),mpctrDiv -> w(),mpctrDiv -> h());
        }
        return ENDTAG;
    }
    OP( Edivfunc)
    {
        if((htnode -> obj)){	
            popRect();
        }
        return ENDTAG;
    }
