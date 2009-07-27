#ifndef __stack__head__matrix_seer
#define __stack__head__matrix_seer
#include "htmltree.h"
//#define   stack_ele_t int  
#define stack_ele_t pHtmlNode  
class __stack
{
public:
    __stack(){_tagtop=-1;}
    int gettagfromstack();
    void _3dhpush(stack_ele_t a);
    //void _3dhpush(int a,char *htdoc);
    void _3dhpush(int a,void *pdrawAttr);
    void _3dhpush(int  a);
    stack_ele_t _3dhpop();
    stack_ele_t _3dhtop();
    int _stackisempty();
    void _printstack();
    int _stackisSpouse(int id);
    pHtmlNode createHtmlNode(int id);
    //pHtmlNode createHtmlNode(int id,attr_t attr);
    //pHtmlNode createHtmlNode(int id,char * value);
    pHtmlNode createHtmlNode(int id,void * pdrawAttr);
    protected:
    stack_ele_t _3dhstack[1024];
    int _tagtop;
private:
};
#endif
