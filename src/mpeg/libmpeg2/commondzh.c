#include "stdio.h"
#include "stdlib.h"
#include <inttypes.h>
void transpose8x8(int16_t * a)
{
    int i;
    int j;
    for(i = 0 ;i < 7 ;i++)
        for(j = i +1; j< 8 ;j++)
        {
            uint16_t tmp = a[i* 8 +j];
            a[i*8 + j] = a[j*8 +i];
            a[j* 8 +i] = tmp;
        }
}
void printrow(uint16_t *a)
{
    int i;
    for(i=0;i<8;i++)
    printf("%d ",(int)a[i]);
    
}
void printcol(uint16_t *a)
{
    int i;
    for(i=0;i<8;i++)
    printf("%d ",(int)a[8*i]);
}  
void print8x8_8(uint8_t *a)
{
    int i;
    int j;
    for(i=0;i<8;i++){
        for(j=0;j<8;j++)
           printf("%d ",a[8*i+j]);
        printf("\n");
    }
}
void print8x8(uint16_t *a)
{
    int i;
    for(i=0;i<8;i++){
        printrow(&a[8*i]);
        printf("\n");
    }
}
