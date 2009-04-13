/*
 * =====================================================================================
 *
 *       Filename:  token.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年12月08日 22时24分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#ifndef __DM_TOKEN_HEADER__
#define __DM_TOKEN_HEADER__
#include <stdio.h>
#include <stdlib.h>
class dmToken
{
    public:
        int tagid;
        char* attrib;
        char* value;
    public:
        dmToken(){}
        dmToken(int t, char*a, char* v):tagid(t),attrib(a),value(v){}
        virtual    ~dmToken(){}
        int& getid() { return tagid;}
        virtual    char*  getAttr(){return attrib;}
        virtual       char*  getValue(){return value;}
};

class dmStartToken:public dmToken
{
    public:
        dmStartToken(int t,char* a){tagid =t; attrib =a; value = NULL;}
        ~dmStartToken(){}
        virtual    char*  getAttr(){return attrib;}
        virtual    char*  getValue(){return NULL;}
};

class dmEndToken:public dmToken
{
    public:
        dmEndToken(int t){tagid =t; attrib =NULL; value = NULL;}
        ~dmEndToken(){}
        virtual    char*  getAttr(){return NULL;}
        virtual    char*  getValue(){return NULL;}
};

class dmTextToken:public dmToken
{
    public:
        dmTextToken(int t,char* v){tagid =t; value = v;}
        ~dmTextToken(){}
        virtual    char*  getAttr(){return NULL;}
        virtual    char*  getValue(){return value;}
    public:
};

class dmSimpleToken:public dmToken
{
    public:
        int tagid;
        char* attrib;
        char* value;
    public:
        dmSimpleToken():tagid(-1),attrib(NULL),value(NULL){}
        dmSimpleToken(int t, char*a, char* v):tagid(t),attrib(a),value(v){}
        ~dmSimpleToken(){}
    public:
        void set(int tagid_, char* attr, char* v){
            tagid = tagid_;
            attrib = attr;
            value = v;
        }
        char*  getAttr(){return attrib;}
        char*  getValue(){return value;}
};
typedef dmToken* dmTokenP;
#endif
