#ifndef DMTYPE_HEADER__
#define DMTYPE_HEADER__

typedef unsigned char U8;
typedef unsigned char* pU8;
typedef unsigned char  Uchar;
typedef unsigned char* pUchar;
typedef unsigned int  U32;


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
//typedef int int3[3];
//typedef int int4[4];
typedef union{
	struct{
		int x;
		int y;
		int z;
	};
	struct{
		int w;
		int h;
		int d;
	};
	int v[3];
}int3;
typedef union{
	struct{
		int x;
		int y;
		int z;
		int a;
	};
	struct{
		int w;
		int h;
		int d;
		int f;
	};
	int v[4];
}int4;

class Color
{
	public:
		union {
			struct {
				U8	r;
				U8	g;
				U8	b;
				U8	a;
			};
			U32 c;
		};
	public:
        Color():c(0){}
		Color(U32 tC):c(tC){}
		//Color(double r, double g, double b, double a){}
		Color(U8 tR, U8 tG, U8 tB, U8 tA):r(tR),g(tG),b(tB),a(tA){}
};
#endif
