#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "string.h"
#include "htmltree.h"
int __stack::gettagfromstack()
{
    if(_tagtop>=0)	
        return _3dhstack[_tagtop]->tagid;
    else
        return -1;
}
void __stack::_3dhpush(stack_ele_t a)
{
    _tagtop++;
    _3dhstack[_tagtop]=a;
}
void __stack::_3dhpush(int a)
{
    _tagtop++;
    _3dhstack[_tagtop]=createHtmlNode(a);
}
void __stack::_3dhpush(int a,void *pdrawAttr)
{
    _tagtop++;
    _3dhstack[_tagtop]=createHtmlNode(a,pdrawAttr);
}
//void __stack::_3dhpush(int a,char *htdoc)
//{
//    _tagtop++;
//    _3dhstack[_tagtop]=createHtmlNode(a,htdoc);
//}
stack_ele_t  __stack::_3dhtop()
{
    if(_tagtop>=0)	
        return _3dhstack[_tagtop];
    else
        return NULL;
}
stack_ele_t  __stack::_3dhpop()
{
    if(_tagtop<0)return NULL;
    _tagtop--;
    return _3dhstack[_tagtop+1];
}
int __stack::_stackisSpouse(int id)
{
    if((_tagtop > -1 ) &&(_3dhstack[_tagtop] -> tagid == id-1)) return 1;
    return 0;
}

int __stack::_stackisempty()
{
    if(_tagtop==-1)return 1;
    return 0;
}
void __stack::_printstack()
{
    int i;
    for(i=0;i<=_tagtop;i++)
        debprintf("%d ",_3dhstack[i] -> tagid);
}
pHtmlNode __stack::createHtmlNode(int id)
{
    //pHtmlNode tmp=(pHtmlNode)malloc(sizeof(HtmlNode)+ sizeof(void*));
    //memset(tmp,0,sizeof(HtmlNode));
    //tmp->tagid=id;
	pHtmlNode tmp = new HtmlNode(id);
    return tmp;
}

//
//pHtmlNode __stack::createHtmlNode(int id,char * value)
//{
//    pHtmlNode tmp=(pHtmlNode)malloc(sizeof(HtmlNode) + sizeof(void*));
//    memset(tmp,0,sizeof(HtmlNode));
//    tmp->tagid=id;
//    attrOfHtmlNode(tmp) = (void *) malloc(sizeof(attrBase_t));    //tmp->attr.value=value;
//    memset(attrOfHtmlNode(tmp),0,sizeof(attrBase_t));
//    return tmp;
//}
//
pHtmlNode __stack::createHtmlNode(int id,void * pdrawAttr)
{
    //pHtmlNode tmp=(pHtmlNode)malloc(sizeof(HtmlNode) + sizeof(void*));
    //memset(tmp,0,sizeof(HtmlNode));
	HtmlNode* tmp = new HtmlNode(id);
    //tmp->tagid=id;
    attrOfHtmlNode(tmp)=pdrawAttr;
    return tmp;
}
