#include <sys/time.h>
double mdtime(int id)
{
	static double start;
	static double end;
	 struct timeval tpstart,tpend;
	 double  timeuse; 
	if(id ==0){
	gettimeofday(&tpstart,NULL);
	}
	else{
	gettimeofday(&tpend,NULL);
	timeuse=(double)1e6*(double)(tpend.tv_sec-tpstart.tv_sec)+	(double)tpend.tv_usec - (double)tpstart.tv_usec;
	 timeuse /= 1e6; 
	 return timeuse;
	}
return 0;
}
