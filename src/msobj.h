#ifndef __MSOBJ_HEADER__
#define __MSOBJ_HEADER__
typedef struct{
    int type;
    void * obj;
}msobj,*pmsobj;
typedef struct _objnode {
    msobj obj;
    struct _objnode *child,*silb;
}objnode,objtree;
#endif
