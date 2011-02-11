
#include "stdio.h"
#include "mdtime.h"
//#include "mmx.h"
int checkab(char *a,char *b, int n)
{
    int res = 1;
    int i;
    for(i=0;i<n;i++)
    {
        if(a[i]!=b[i]) {
        res = 0;
        return 0;
        }
    }
    return 1;
}
    initab(char *a ,char *b,int n)
    {
       int i;
       for(i=0;i< n;i++)
       {
        a[i] = i% 200;
        b[i] =0;
       }
    }
int main()
{
    char *a;
    char *b;
    int i;
    int ia,ib;
    double t3;
    int n = 1024* 64;
    int res =0;
    a =(char *)malloc(1024 * 64+ 128) ;
    ia = (int )a;
    if(ia & 15){
    ia += 16;
    a =(ia+16) & (0xffffff00);
    }
    b = (char *)malloc(1024 * 64+ 128) ;
    ib =(int )b; 
    if(ib & 15){
    ib += 16;
    b = (ib) & (0xffffff00);
    }
initab(a,b,1024*64);
mtime(0);    
memcpy(b,a,1024*64);
t3= mtime(1);
res = checkab(a,b,n);
printf("memcpy : %f\nres:%d \n",t3,res);

initab(a,b,1024*64);
mtime(0);   
#if 1
    for(i =0 ;i< 1024* 8;i++){
    asm("movq %1,%%mm1;"
    "movq %%mm1,%0"
    :"=m"(b[i*8])
    :"m"(a[i*8])
    );
    }
asm("emms");
    #endif
t3= mtime(1);
res = checkab(a,b,n);
printf("mmx : %f\nres:%d \n",t3,res);

initab(a,b,1024*64);
mtime(0);    
    for(i =0 ;i< 1024* 4;i++){
    asm("movdqa %1,%%xmm1;"
    "movapd %%xmm1,%0"
    :"=m"(b[i*16])
    :"m"(a[i*16])
    );
    }
t3= mtime(1);
res = checkab(a,b,n);
printf("sse : %f\nres:%d \n",t3,res);





}
