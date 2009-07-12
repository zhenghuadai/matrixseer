#include <stdlib.h>
#include <math.h>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define RAD_TO_DEG (180.0/M_PI)
#define DEG_TO_RAD (M_PI/180.0)
float * getarc()
{
	float *p ;
	int i;
	float r=sqrt(2);
	double alpha;
	double stepa;
	double base;
	p=(float *) malloc(22*sizeof(float));
	alpha=2*M_PI/3;
	stepa=M_PI-2*(M_PI-alpha);
	stepa=stepa/10;
	r=1/cos(M_PI-alpha);
	base=-r*sin(alpha);
	for(i=0;i<11;i++)
	{
		p[2*i]=r*cos(alpha);
		p[2*i+1]=(-r*sin(alpha)-base)*4;
		alpha-=(stepa);
	}
	return p;
}
int main()
{
	float * p;
	int i;
	p=getarc();
	for(i=0;i<11;i++)
	printf("%f,%f,",p[2*i],p[2*i+1]);
}
