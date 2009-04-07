#ifndef __CSS_HEADER__
#define __CSS_HEADER__
#include "stdio.h"

typedef struct {

}css_t;

typedef struct {
int id;
char * value;
}cssEle_t;

typedef struct {
int len;
cssEle_t *value;
}cssAttr_t;

typedef struct {
char *name;
int len;
cssEle_t *value;
}cssID_t;

typedef struct {
char *name;
int len;
cssEle_t *value;
}cssClass_t;

typedef struct {
int outline_color;
int outline_style;
int outline_width;
}cssOutline_t;

typedef struct{
int x,y,w,h;
}cssRect_t;

typedef  cssRect_t shape_t;
typedef struct {
int position;
int left,bottom,right,top;
shape_t clip;
int overflow;
int vertical_align;
int z_index;
}cssPosition_t;
typedef enum cssSelectorID_{
#include "cssSelectorID.h"
}cssSelectorID_t;
#endif
