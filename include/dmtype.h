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

#endif
