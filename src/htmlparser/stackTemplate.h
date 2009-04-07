#ifndef __stackTemplate__head__matrix_seer
#define __stackTemplate__head__matrix_seer
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#define STACK_MAX_SIZE 1024
#define isEMPTY 1
#define notEMPTY 0
#define isFULL 1
#define notFULL 0
template <typename  stack_ele_t>
class stack_t
{
    public:
        stack_t(){_tagtop=-1;}
        void _3dhpush(stack_ele_t a)
        {
            _tagtop++;
            _3dhstack[_tagtop]=a;
        }
        stack_ele_t  _3dhtop()
        {
            if(_tagtop>=0)	
                return _3dhstack[_tagtop];
            else
                return NULL;
        }
        stack_ele_t  _3dhpop()
        {
            if(_tagtop<0)return NULL;
            _tagtop--;
            return _3dhstack[_tagtop+1];
        }

        int _isempty()
        {
            if(_tagtop==-1)return isEMPTY;
            return notEMPTY;
        }
        
        int _isfull()
        {
            if(_tagtop ==   STACK_MAX_SIZE) return isFULL;
            return notFULL;
        }
        void _printstack()
        {
        }
    protected:
        stack_ele_t _3dhstack[STACK_MAX_SIZE];
        int _tagtop;
};
#endif
