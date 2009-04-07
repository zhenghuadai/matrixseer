#ifndef __Simplestack__head__matrix_seer
#define __Simplestack__head__matrix_seer
#include "htmltree.h"
//#define   SStack_ele_t int 
#ifdef SStack_ele_t
#undef SStack_ele_t
#endif
typedef struct
{
    int x;
    int y;
    int w;
    int h;
}Rect_t;
#define SStack_ele_t Rect_t  
class __SimpleStack
{
public:
    __SimpleStack(){_tagtop=-1;}
    void push(SStack_ele_t a){_tagtop++;_stack[_tagtop]=a;};
    SStack_ele_t _3dhpop(){};
    SStack_ele_t _3dhtop(){};
    int _3dhpop(SStack_ele_t* a){
        if(_tagtop >= 0)
        {
            *a = _stack[_tagtop--];
            return 1;
        }else 
            return 0;
    };
    int _3dhtop(){
        if(_tagtop >= 0)
        {
            *a = _stack[_tagtop];
            return 1;
        }else 
            return 0;
    };
    int _stackIsEmpty(){
        if(_tagtop==-1)return 1;
        return 0;

    };
    void _printstack(){};
private:
    SStack_ele_t _stack[1024];
    int _tagtop;
};
#endif
