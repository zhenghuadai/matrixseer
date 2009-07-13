#include "htmltree.h"
#include "html.h"
#include "dmtype.h"
#include "ctrinput.h"
#include "ctrA.h"
#include "ctrP.h"
#include "ctrDiv.h"
#include "ctrText.h"
#include "ctrbutton.h"
#include "display.h"
#include "debprintf.h"
#include "css.h"
#include "stackTemplate.h"

extern int getCharsWidth(char *s);
extern int2 getstrWH(char *);
extern int rowHeight;
extern int charWidth;

    template<typename T>
void setCSS(T *pctrP,cssAttr_t *pcss)
{
    int i;
    for(i=0;i<pcss -> len; i++){
        switch(pcss->value[i].id){
            case leftID:
                pctrP->setFixX(atoi(pcss->value[i].value));
                break;
            case topID:
                pctrP->setFixY(atoi(pcss->value[i].value));
                break;
            default:
                break;
        }
    }
}

HtmlUlNode:: HtmlUlNode(pHtmlNode pHtp, dmToken* t)
{
    tagid = ULID;
    attr = (void *) malloc(sizeof(attrBase_t));    //tmp->attr.value=value;
    memset(attr,0,sizeof(attrBase_t));
    /*attr = new attrBase_t();*/

    pHtp ->addChild(this);
    int x, y;
    attrBase_t *pAttrp = (attrBase_t*) getAttrBaseP(pHtp);
    GetCurXYFromPattr(x,y,pAttrp);
    ctrP * pctrP = new ctrP(x,y ,0);
    obj = (void*) pctrP;
    pctrP -> begin();
}

void HtmlUlNode:: endParse()
{
    pHtmlNode pHtc = this; 
    pHtmlNode pHtp = this -> parent; 
    pTAGattr_t *paattr = (pTAGattr_t*) pHtc ->attr;
    updateSelfAttrEnter(paattr);
    paattr->w += 10;
    paattr->h += 10;

    ctrP* pctrP=(ctrP*) (pHtc -> obj );
    pctrP -> size(paattr->w  ,paattr->h);
    debprintf("ctrP x:%d y:%d w:%d h:%d\n",pctrP->x(),pctrP->y(),paattr -> w , paattr->h);
    pctrP -> labelcolor(216);
    pctrP -> end();
    pHtc -> wife = tagid + 1;
    updateParentHtmlNode(pHtp,pHtc);
}

void  HtmlUlNode::processValue(dmToken * t)
{
}

/*******************************************************************
 *
 *
 * ****************************************************************/
HtmlTdNode::HtmlTdNode(pHtmlNode pHtp, dmToken* t)
{
    int x, y;
    //this -> tagid = TDID;
    this -> tagid = t->tagid;//TDID;
    trattr_t *trattr =(trattr_t*) getAttrBaseP(pHtp);

    tdattr_t *tdattr;
    htmlparser::tdAttrFunc(t->attrib,(void**)&tdattr);
    //curHtml->_3dhpush(tagid,(void*)tdattr);
    //
    this->attr = tdattr;
    pHtp ->addChild(this); 
    debprintf(" td align :%d\n",tdattr->align);
    tdattr -> colID = trattr -> cols;
    tdattr -> colsMaxW =(int *)&( trattr -> colsMaxW[trattr -> cols]);
#ifdef DZHDEB 
    char *ttt = (char *)malloc(20);
    sprintf(ttt,"td%d.%d",trattr->rowID,tdattr -> colID);
#endif
    GetCurXYFromPhtmlNode( x,y, pHtp);
    ctrDiv * pctrDiv = new ctrDiv(x,y,1,1,0);
    debprintf("td size: %d %d %d %d\n",x,y,10 ,10);
    this -> obj = (void*) pctrDiv;
    pctrDiv -> color(10);
    pctrDiv -> begin();
}

void HtmlTdNode:: endParse()
{
    pHtmlNode pHtc = this; 
    pHtmlNode pHtp = this -> parent; 
    trattr_t * tr =(trattr_t*) getAttrBaseP(pHtp);
    tdattr_t * td =(tdattr_t*) getAttrBaseP(pHtc);

    updateSelfHtmlNode(pHtc);
    updateParentHtmlNode(pHtp,pHtc);
    if (td -> h  >  tr -> h) tr->h = td->h;
    tr->w += td->w;
    //if((td -> w) > *(td -> colsMaxW)) *(td -> colsMaxW) = td -> w;
    if((td -> w) > (tr -> colsMaxW[td->colID].length)) 
        tr -> colsMaxW[td->colID].length = td -> w;
    (tr-> cols) ++;
    pHtc -> wife = tagid + 1;
    //HNinsert(pHtp,pHtc);
    int w,h;
    GetCurWHFromhtmlNode(w,h,pHtc);
    debprintf("TD resize: w:%d h:%d\n",w,h);
    ctrDiv* pctr=(ctrDiv*) (pHtc -> obj );
    pctr -> end();
    pctr -> size( w,h);
    //return 1;

}

void  HtmlTdNode::processValue(dmToken * t)
{
}

/*******************************************************************
 *
 *
 * ****************************************************************/
HtmlTrNode::HtmlTrNode(pHtmlNode pHtp, dmToken* t)
{
    int x,y;
    tagid = t -> tagid;
    pHtmlNode tableNodep = pHtp;//curHtml->_3dhtop();
    tableattr_t * tableattr = (tableattr_t*)getAttrBaseP(tableNodep);
    trattr_t *trattr;
    trattr = (trattr_t*)malloc(sizeof(trattr_t));
    memset(trattr,0,sizeof(trattr_t));
    trattr -> colsMaxW = tableattr -> colsW;
    trattr -> currentRowH = &(tableattr -> rowsH[tableattr -> rows]);
    attrOfHtmlNode(this) = trattr;
    pHtp ->addChild(this); 

#ifdef DZHDEB 
    char *ttt = (char *)malloc(220);
#endif
    GetCurXYFromPhtmlNode( x,y, tableNodep);
    ctrDiv * pctrDiv = new ctrDiv(x,y,10,10,0);
    this -> obj = (void*) pctrDiv;
    debprintf("tr size: %d %d %d %d\n",x,y,10 ,10);
    pctrDiv -> begin();
    pctrDiv -> color(10);

}

void HtmlTrNode:: endParse()
{
    pHtmlNode pHtc = this; 
    pHtmlNode pHtp = this -> parent; 
    tableattr_t * table =(tableattr_t*) getAttrBaseP(pHtp);
    trattr_t * tr =(trattr_t*) getAttrBaseP(pHtc);
    pHtc -> wife = tagid;
    table->rowsH[table->rows].length = tr->h;
    tr -> rowID = table -> rows;
    (table -> rows) ++;
    if(tr ->cols > table -> cols) table -> cols = tr -> cols;

    int w,h;
    updateParentHtmlNode(pHtp,pHtc);
    GetCurWHFromhtmlNode(w,h,pHtc);
    debprintf("TR resize : w: %d h:%d\n",w,h);
    ctrDiv* pctr=(ctrDiv*) (pHtc -> obj );
    pctr -> size( w,h);
    pctr -> end();

}


void  HtmlTrNode::processValue(dmToken * t)
{
}


/*******************************************************************
 *
 *
 * ****************************************************************/
HtmlTableNode::HtmlTableNode(pHtmlNode pHtp, dmToken* t)
{
    int x,y;
    tagid = t-> tagid;
    tableattr_t *tableattr;
    htmlparser::tableAttrFunc(t->attrib,(void**)&tableattr);
    this->attr = tableattr;
    pHtp ->addChild(this); 
    GetCurXYFromPhtmlNode( x,y, pHtp);
    ctrDiv * pctrDiv = new ctrDiv(x,y,1,1,0);
    debprintf("Table Size:%d %d %d %d\n",x,y,1,1);
    this -> obj = (void*) pctrDiv;
    //pctrDiv -> color(tableattr -> bgcolor);
    pctrDiv -> color(10);
    pctrDiv -> begin();

}

void HtmlTableNode:: endParse()
{
    void resetTablePos(pHtmlNode tableP);

    pHtmlNode pHtc = this; 
    pHtmlNode pHtp = this -> parent; 
    pHtc -> wife = tagid;
    tableattr_t *tableattr = ( tableattr_t *)getAttrBaseP(pHtc);
    int i;
    int w,h;
    int pos[8];
    getRect(pos);
    debprintf("\ntableSize: rows %d x cols %d\n",tableattr->rows,tableattr->cols);
    debprintf("table width = %d\%",tableattr -> w );
    debprintf("rect :%d %d %d %d \n",pos[0],pos[1],pos[4],pos[5]);
    //upadte the width
    gridPos_t *src = tableattr -> colsW; 

    int tmpW =0;
    for(i=0;i< tableattr -> cols;i++){
        tmpW += src[i].length; 
    }
    if(tableattr -> w <0){
        // warning : the w will be rewrite ,so it shoulde not be resize
        // the width is x%
        int tmpm = (pos[4]  -  tmpW);
        if(tmpm > 0){
            int avm = tmpm / tableattr->cols;
            for(i=0;i< tableattr -> cols;i++){
                src[i].length += avm; 
            }
        }
    }

    debprintf("colsW[%d %d] ",0,src[0].length);
    for(i=1;i< tableattr -> cols;i++){
        src[i].offset = src[i-1].offset + src[i-1].length;
        debprintf("[%d %d] ",i,src[i].length);
    }
    tableattr -> w = src[i-1].length + src[i-1].offset;
    //update the hight
    src = tableattr -> rowsH;
    debprintf("\nrowsH[%d %d] ",0,src[0].length);
    for(i=1;i< tableattr -> rows;i++){
        src[i].offset = src[i-1].offset + src[i-1].length;
        debprintf("[%d %d] ",i,src[i].length);
    }
    tableattr -> h = src[i-1].length + src[i-1].offset;

    debprintf("TABLE: w:%d h:%d\n",tableattr->w,tableattr->h);
    //UPDATEatEnd; 
    updateParentHtmlNode(pHtp,pHtc);
    GetCurWHFromhtmlNode(w,h,pHtc);
    debprintf("TABLE resize : w:%d h:%d\n",w,h);
    ctrDiv* pctr=(ctrDiv*) (pHtc -> obj );
    if(tableattr -> align == ALIGNCENTER){

        int shiftR = (pos[4] - tableattr -> w )/2;
        pctr -> setx(pctr->x() + shiftR);
    }
    pctr -> end();
    pctr -> size( w,h);
    resetTablePos(pHtc);
}

void  HtmlTableNode::processValue(dmToken * t)
{
}


/*******************************************************************
 *
 *
 * ****************************************************************/
extern void getRect(int *);

HtmlDivNode::HtmlDivNode(pHtmlNode pHtp, dmToken* t)
{
    tagid = t-> tagid;
    divattr_t *p_divattr;
    htmlparser::DivAttr(t->attrib,(void**)&p_divattr);
    this -> attr = p_divattr;
    pHtmlNode pHtc = this; 
    pHtp ->addChild(this);
    //HNinsert(pHtp,pHtc);
    int x,y,h,w;
    int pos[8];
    void setdefaultfont(void);
    setdefaultfont();
    ctrDiv * pctrDiv;
    getRect(pos);
    if(p_divattr -> startx ==2){
        x = pos[0]; y= pos[1]; w= pos[4]/2; h =pos[5];   
        pctrDiv = new ctrDiv(x,y,w,h,0);
        pctrDiv -> setFixWH();
    }
    else if(p_divattr -> startx ==1){
        x = pos[4]/2; y= pos[1]; w= pos[4]/2; h =pos[5];   
        pctrDiv = new ctrDiv(x,y,w,h,0);
        pctrDiv -> setFixWH();
    }
    else{
        GetCurXYFromPhtmlNode(x,y,pHtp);
        pctrDiv = new ctrDiv(x,y,0,0,0);
        w = pos[4];
        h = pos[5];
    }
    debprintf("DIV:%d %d %d %d \n",x,y,w,h);
    pHtc -> obj = (void*) pctrDiv;
    pctrDiv -> begin();
    pushRect();
    setRect(x,y,w,h);
}

void HtmlDivNode:: endParse()
{
    pHtmlNode pHtc = this; 
    pHtmlNode pHtp = this -> parent; 
    pHtc -> wife = tagid;

    pTAGattr_t *paattr = (pTAGattr_t*) pHtc ->attr;
    updateSelfAttrEnter(paattr);
    int w,h;
    ctrDiv* pctrDiv=(ctrDiv*) (pHtc -> obj );
    GetCurWHFromhtmlNode(w,h,pHtc);
    //updateParentHtmlNode(pHtp,pHtc);
    debprintf("Divsize  %d %d\n",paattr -> w +5, paattr->h);
    pctrDiv -> labelcolor(216);
    pctrDiv -> end();
    if(!(pctrDiv -> isFixWH())){
        pctrDiv -> setWH(w,h);
        //updateParentHtmlNode(pHtp,pHtc);
    }
    updateParentHtmlNode(pHtp,pHtc);
    pHtc -> wife = tagid;
    //HNinsert(pHtp,pHtc);
    popRect();
}

void  HtmlDivNode::processValue(dmToken * t)
{
}


/*******************************************************************
 *
 *
 * ****************************************************************/
extern void getRect(int *);

extern int getCharsWidth(char *s);
extern cssAttr_t* getcssFromID(char*  ,char* tag=NULL );
extern cssAttr_t* getcssFromClass(char*  ,char* tag=NULL );

HtmlPNode::HtmlPNode(pHtmlNode pHtp, dmToken* t)
{
    tagid = t-> tagid;
    pHtp ->addChild(this);
    pHtmlNode pHtc = this; 

    int x=0,y=0;
    pTAGattr_t *p_pattr;
    htmlparser::pAttr(value,(void**)&p_pattr);
    this -> attr = p_pattr;
    debprintf("[%s]  \n",p_pattr->p_id);
    cssAttr_t * pcss = NULL;
    if(p_pattr ->p_id) {
        pcss = getcssFromID(p_pattr -> p_id);
    }
    if(p_pattr ->p_class) {
        pcss = getcssFromClass(p_pattr -> p_class,"p.");
    }
    attrBase_t *pAttrp = (attrBase_t*) getAttrBaseP(pHtp);
    GetCurXYFromPattr(x,y,pAttrp);
    ctrP * pctrP = new ctrP(x,y ,0);
    //ctrP * pctrP = new ctrP(20,20 ,200,200,"ctrP");
    if(pcss){
        /*
           debprintf("%d %d %s\n",pcss -> len,pcss->value[0].id,pcss->value[1].value);
           debprintf("{%d %d}\n",atoi(pcss->value[1].value),atoi(pcss->value[2].value));
           x = atoi(pcss->value[1].value);
           y = atoi(pcss->value[2].value);
           pctrP -> setFixXY(x,y);
           */
        setCSS(pctrP,pcss);
        //pushRect();
        //setRect(pctrP->x(),pctrP->y(),getRectW() - pctrP->x(),getRectH());

    }
    debprintf("1 ctrP %d %d x:%d y:%d \n",x,y,pctrP->x(),pctrP->y());
    pHtc -> obj = (void*) pctrP;
    pctrP -> begin();
}

void HtmlPNode:: endParse()
{
    pHtmlNode pHtc = this; 
    pHtmlNode pHtp = this -> parent; 
    pHtc -> wife = tagid;
    pTAGattr_t *paattr = (pTAGattr_t*) pHtc ->attr;
    updateSelfAttrEnter(paattr);
    paattr->w += 10;
    paattr->h += 10;

    ctrP* pctrP=(ctrP*) (pHtc -> obj );
    //if(pctrP -> PostionFlag) popRect();
    pctrP -> size(paattr->w  ,paattr->h);
    debprintf("ctrP x:%d y:%d w:%d h:%d\n",pctrP->x(),pctrP->y(),paattr -> w , paattr->h);
    pctrP -> labelcolor(216);
    //  pctrP -> hide();
    pctrP -> end();
    pHtc -> wife = tagid;
    //HNinsert(pHtp,pHtc);
    updateParentHtmlNode(pHtp,pHtc);
    enterHtmlNode(pHtp);
}

void  HtmlPNode::processValue(dmToken * t)
{
}


/*******************************************************************
 *
 *
 * ****************************************************************/
HtmlANode::HtmlANode(pHtmlNode pHtp, dmToken* t)
{
    tagid = t-> tagid;
    pHtp ->addChild(this);
    pHtmlNode pHtc = this; 

    aattr_t *p_aattr;
    htmlparser::aAttr(t->attrib,(void**)&p_aattr);
    debprintf(" afunc href:%s\n",p_aattr ->href);
    this -> attr = p_aattr;
    int x=0,y=0;
    attrBase_t *pAttrp = (attrBase_t*) getAttrBaseP(pHtp);
    GetCurXYFromPattr(x,y,pAttrp);
    ctrP * pctrP = new ctrP(x,y ,0);
    pHtc -> obj = (void*) pctrP;
    pctrP -> begin();
}

void HtmlANode:: endParse()
{
    pHtmlNode pHtc = this; 
    pHtmlNode pHtp = this -> parent; 
    pHtc -> wife = tagid;

    pTAGattr_t *paattr = (pTAGattr_t*) pHtc ->attr;
    updateSelfAttrEnter(paattr);
    paattr->w += 10;
    paattr->h += 10;

    ctrP* pctrP=(ctrP*) (pHtc -> obj );
    pctrP -> size(paattr->w  ,paattr->h);
    debprintf("ctrP x:%d y:%d w:%d h:%d\n",pctrP->x(),pctrP->y(),paattr -> w , paattr->h);
    pctrP -> labelcolor(216);
    pctrP -> end();
    updateParentHtmlNode(pHtp,pHtc);
    enterHtmlNode(pHtp);

}

void  HtmlANode::processValue(dmToken * t)
{
}


/*******************************************************************
 *
 *
 * ****************************************************************/
HtmlSimpleANode::HtmlSimpleANode(pHtmlNode pHtp, dmToken* t)
{
    tagid = t-> tagid;
    pHtp ->addChild(this);
    pHtmlNode pHtc = this; 

    {
        aattr_t *p_aattr;
        htmlparser::aAttr(t->attrib,(void**)&p_aattr);
        debprintf(" afunc href:%s\n",p_aattr ->href);
        this -> attr = p_aattr;
    }
    {
        pHtc->value=t->value;
        int2 wh = getstrWH(value);
        updateCurWH(pHtc,wh.x,rowHeight);
    }
    {
        aattr_t *paattr = (aattr_t*) getAttrBaseP(pHtc);
        attrBase_t *pobjP = (attrBase_t *) getAttrBaseP(pHtp);
        updateSelfAttrEnter(paattr);
        ctrA * pctrA = new ctrA(pobjP->curW,pobjP->h,paattr->w,paattr->h ,pHtc ->value);
        updateParentHtmlNode(pHtp,pHtc);
        debprintf("A:%d %d \n",pobjP->curW,pobjP->h);
        void setdefaultfont(void);
        pctrA -> labelcolor(216);
        pHtc -> obj = (void*) pctrA;
        setdefaultfont();
    }
}

void HtmlSimpleANode:: endParse()
{
    return ;
}

void  HtmlSimpleANode::processValue(dmToken * t)
{
}


/*******************************************************************
 *
 *
 * ****************************************************************/
HtmlCenterNode::HtmlCenterNode(pHtmlNode pHtp, dmToken* t)
{
    tagid = t-> tagid;
    pHtp ->addChild(this);
    pHtmlNode pHtc = this; 

    int x,y;
    this ->attr = (void *) malloc(sizeof(attrBase_t));    //tmp->attr.value=value;
    memset(attr,0,sizeof(attrBase_t));
    GetCurXYFromPhtmlNode(x,y,pHtp);
    ctrDiv * pctrDiv;
    pctrDiv = new ctrDiv(x,y,0,0,0);
    pHtc -> obj = (void*) pctrDiv;
    attrBase_t* pattr =(attrBase_t*)  pHtc -> attr;
    debprintf("center curW:%d\n",pattr->curW);
    pctrDiv -> begin();
}

void HtmlCenterNode:: endParse()
{
    pHtmlNode pHtc = this; 
    pHtmlNode pHtp = this -> parent; 
    pHtc -> wife = tagid;

    pTAGattr_t *paattr = (pTAGattr_t*) pHtc ->attr;
    updateSelfAttrEnter(paattr);
    int w,h;
    ctrDiv* pctrDiv=(ctrDiv*) (pHtc -> obj );
    GetCurWHFromhtmlNode(w,h,pHtc);
    pctrDiv -> end();
    int pos[8];
    getRect(pos);
    int x;
    x = (pos[4] - w )/2;
    debprintf("centerX:%d,%d\n",pos[4],w);
    pctrDiv -> setx(x);
    pctrDiv -> setWH(w,h);
    updateParentHtmlNode(pHtp,pHtc);
    return ;
}

void  HtmlCenterNode::processValue(dmToken * t)
{
}

/*******************************************************************
 *
 *
 * ****************************************************************/
HtmlInputNode::HtmlInputNode(pHtmlNode pHtp, dmToken* t)
{
    tagid = t-> tagid;
    pHtp ->addChild(this);
    pHtmlNode pHtc = this; 

    inputattr_t *p_inputattr;
    htmlparser::InputAttr(t->attrib,(void**)&p_inputattr);
    this->attr = p_inputattr;
    int x,y;
    GetCurXYFromPhtmlNode(x,y,pHtp);
#ifndef __HTMLMAIN 
    switch(p_inputattr -> type)
    {
        case INPUTINPUT:
            {
                if(! (p_inputattr->w)) p_inputattr->w = 200;
                if(! (p_inputattr->h)) p_inputattr->h = 22;
                ctrInput * pctrInput0 = new ctrInput(x,y,200,22,0);
                pHtc -> obj = (void *) pctrInput0;
            }
            break;
        case INPUTBUTTON:
            {
                char * pbvalue =strdup(p_inputattr -> value);// (char *)malloc(sizeof(pstr)+1);
                if(! (p_inputattr->w)) p_inputattr->w = 100;
                if(! (p_inputattr->h)) p_inputattr->h = 22;
                ctrButton *pctrButton0 = new ctrButton(x,y,100,20,pbvalue);
                pHtc ->obj = (void *) pctrButton0;
            }
            break;
    }
#endif
    updateParentHtmlNode(pHtp,pHtc);
}

/*******************************************************************
 *
 *
 * ****************************************************************/
HtmlImgNode::HtmlImgNode(pHtmlNode pHtp, dmToken* t)
{
    tagid = t-> tagid;
    pHtp ->addChild(this);
    pHtmlNode pHtc = this; 

    imgattr_t* p_imgattr;//=(imgattr_t*)malloc(sizeof(imgattr_t));
    htmlparser:: ImgAttr(t->attrib,(void**)&p_imgattr);
    if((p_imgattr -> w ==0) || (p_imgattr ==0)){
        p_imgattr -> w = 100;
        p_imgattr -> h = 100;
    }
    this -> attr = p_imgattr;
    updateParentHtmlNode(pHtp,pHtc);
}

/*******************************************************************
 *
 *
 * ****************************************************************/
HtmlBrNode::HtmlBrNode(pHtmlNode pHtp, dmToken* t)
{
    tagid = t-> tagid;
    pHtp ->addChild(this);
    pHtmlNode pHtc = this; 
    if( pHtp -> attr){
        attrBase_t *pAttrp = (attrBase_t *) getAttrBaseP(pHtp);
        enterAttr(pAttrp);
    }

}