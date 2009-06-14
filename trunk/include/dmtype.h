#ifndef DMTYPE_HEADER__
#define DMTYPE_HEADER__
typedef union{
    struct{
        int x;
        int y;
    };
    struct{
        int w;
        int h;
    };
}int2;
typedef int int3[3];
typedef int int4[4];
#endif
