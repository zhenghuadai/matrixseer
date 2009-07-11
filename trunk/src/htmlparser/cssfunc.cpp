#include <stdio.h>
#include "stdlib.h"
#include "tagattr.h"
#include "stack.h"
#include "html.h"
#include "htmltree.h"
#include "string.h"
#include "debprintf.h"
#include "css.h"
const static char * allattrs[]  = {
#include "cssSelector.h" 
};
const int allattrNum = sizeof(allattrs)/sizeof(const  char*);
/*
typedef enum cssSelectorID_{
#include "cssSelectorID.h"
}cssSelectorID_t;
*/
int cssTagpos[27] = {
   0,1,30,41,43,45,52, 52,53,53,53,53,60,69,69,75,90,
// a,b, c,d, e ,f ,g , h, i, j, k ,l ,m ,n, o, p ,q ,
   91,93,99,105,106,110,114,114,114,115
// r, s, t, u,  v,   w, x,y,z
};
cssID_t  cssID[100];
cssID_t  cssClass[100];
int cssIDs = 0;
int cssClasss = 0;

#define UPPERC(a) ((a) > 'Z'? (a) - 32: (a) )
#define DOWNC(a) ((a) > 'Z'? (a) : (a)+32 )

#define getName getNamefunc(pcur,buf) 
void getNamefunc(char* &pcur,char* buf)
{
    int   i=0;
    //while((*pcur)&&(*pcur==' ' ))pcur ++;
    while(  (*pcur)&&((*pcur==' ' )||
            (((*pcur<'A')||(*pcur>'z'))&&
              (*pcur!='.')&&(*pcur!='#')&&(*pcur!='@'))))pcur ++;
    while((*pcur) &&(*pcur !='{')&&(*pcur!=' ')) {
        buf[i++] = *pcur++; 
    }
    buf[i]=0;
    while((*pcur)&&(*pcur!='{' ))pcur ++;
    pcur ++;
}

#define getTag getTagfunc( pcur,buf,tag)
void getTagfunc(char* &pcur,char* buf,int &tag)
{
    int i;
    i=0;
    while((*pcur)&&(*pcur ==' ')) pcur ++;
    while((*pcur) &&(*pcur != ' ') &&(*pcur !=':')) {
        buf[i++] = *pcur; pcur++;
    }
    buf[i] = 0;
    tag = -1;
    debprintf("{%c %d %d }",buf[0],cssTagpos[DOWNC(buf[0])-'a'],cssTagpos[DOWNC(buf[0]) -'a'+1]);
    for(i =cssTagpos[DOWNC(buf[0])- 'a'];i<cssTagpos[DOWNC(buf[0]) -'a'+1];i++)
    {
        int ret =strcasecmp(buf,allattrs[i]);
        if(ret ==0){
            tag = i;
            break;
        }
    }
}

#define getValue getValuefunc(pcur,buf) 
void getValuefunc(char* &pcur,char* buf)
{
    int i;
    while((*pcur)&&(*pcur !=':')) pcur ++;
    pcur++ ;
    i = 0;
    while((*pcur)&&(*pcur ==' ')) pcur ++;
    if(*pcur == '"'){
        pcur++ ;
        while((*pcur) &&(*pcur != '"') ) {
            buf[i++] = *pcur; pcur++;
        }
        if(*pcur) pcur ++ ;
    }
    else{
        while((*pcur) &&(*pcur != '}')&&(*pcur!=';') ) {
            buf[i++] = *pcur; pcur++;
        }
    }
    if(*pcur == ';')pcur ++;
    buf[i]=0;
}

void findHash()
{
    int i;
    char buf[1024];
    const char * pcur;
    for(i = 0;i< allattrNum;i++){
        pcur = (const char *)allattrs[i];
        int len = strlen(pcur);
        char c1 = pcur[0] - 'a';
        char c2 = pcur[len/2] -'a';
        char c3 = pcur[len-1] -'a' ;
        char c4 = pcur[len-2] -'a';
        char c5 = pcur[2] -'a';
        unsigned char c = (c1 << 4)  ^ (( (~c2 << 3) ^(c3<<1) ^(~c4)) );
        debprintf("%3d,%s\n",c,pcur);
    }
}
int parseCss(char *attrstr,void ** pobj)
{
    int tag;
    char * pcur = attrstr;
    char buf[1024];
    cssEle_t cssbuf[256];
    int cssbuflen = 0;
    cssID_t *pcss;
    int *cssS=&cssIDs;
    int i;
    if(attrstr ==NULL) return 1;
    while((*pcur))
    {
        getName;
        if(strstr(buf,".") != NULL) {
            pcss = cssClass;
            cssS = &cssClasss;
        }else if(strstr(buf,"#")!=NULL){
            pcss = cssID;
            cssS = &cssIDs;
        }else if(buf[0] == '@'){
            continue;    
        }else if((buf[0] >= 'A')&&(buf[0] <= 'z')){
            pcss = cssClass;
            cssS = &cssClasss;
        }else{
           continue;//wrone 
        }
        //cssID[cssIDs] .name = strdup(buf);
        pcss[*cssS] .name = strdup(buf);
        cssbuflen =0;
        debprintf("\n%s",buf);
        while((*pcur)&&(*pcur !='}'))
        {
            getTag;
            cssbuf[cssbuflen].id =tag;
            debprintf("[%d %s:",tag,buf);
            switch(tag){
                case azimuthID:
                    getValue;
                    break;
                case backgroundID:
                    getValue;
                    break;
                case background_attachmentID:
                    getValue;
                    break;
                case background_colorID:
                    getValue;
                    break;
                case background_imageID:
                    getValue;
                    break;
                case background_positionID:
                    getValue;
                    break;
                case background_repeatID:
                    getValue;
                    break;
                case borderID:
                    getValue;
                    break;
                case border_bottomID:
                    getValue;
                    break;
                case border_bottom_colorID:
                    getValue;
                    break;
                case border_bottom_styleID:
                    getValue;
                    break;
                case border_bottom_widthID:
                    getValue;
                    break;
                case border_collapseID:
                    getValue;
                    break;
                case border_colorID:
                    getValue;
                    break;
                case border_leftID:
                    getValue;
                    break;
                case border_left_colorID:
                    getValue;
                    break;
                case border_left_styleID:
                    getValue;
                    break;
                case border_left_widthID:
                    getValue;
                    break;
                case border_rightID:
                    getValue;
                    break;
                case border_right_colorID:
                    getValue;
                    break;
                case border_right_styleID:
                    getValue;
                    break;
                case border_right_widthID:
                    getValue;
                    break;
                case border_spacingID:
                    getValue;
                    break;
                case border_styleID:
                    getValue;
                    break;
                case border_topID:
                    getValue;
                    break;
                case border_top_colorID:
                    getValue;
                    break;
                case border_top_styleID:
                    getValue;
                    break;
                case border_top_widthID:
                    getValue;
                    break;
                case border_widthID:
                    getValue;
                    break;
                case bottomID:
                    getValue;
                    break;
                case caption_sideID:
                    getValue;
                    break;
                case clearID:
                    getValue;
                    break;
                case clipID:
                    getValue;
                    break;
                case colorID:
                    getValue;
                    break;
                case contentID:
                    getValue;
                    break;
                case counter_incrementID:
                    getValue;
                    break;
                case counter_resetID:
                    getValue;
                    break;
                case cueID:
                    getValue;
                    break;
                case cue_afterID:
                    getValue;
                    break;
                case cue_beforeID:
                    getValue;
                    break;
                case cursorID:
                    getValue;
                    break;
                case directionID:
                    getValue;
                    break;
                case displayID:
                    getValue;
                    break;
                case elevationID:
                    getValue;
                    break;
                case empty_cellsID:
                    getValue;
                    break;
                case floatID:
                    getValue;
                    break;
                case fontID:
                    getValue;
                    break;
                case font_familyID:
                    getValue;
                    break;
                case font_sizeID:
                    getValue;
                    break;
                case font_styleID:
                    getValue;
                    break;
                case font_variantID:
                    getValue;
                    break;
                case font_weightID:
                    getValue;
                    break;
                case heightID:
                    getValue;
                    break;
                case leftID:
                    getValue;
                    break;
                case letter_spacingID:
                    getValue;
                    break;
                case line_heightID:
                    getValue;
                    break;
                case list_styleID:
                    getValue;
                    break;
                case list_style_imageID:
                    getValue;
                    break;
                case list_style_positionID:
                    getValue;
                    break;
                case list_style_typeID:
                    getValue;
                    break;
                case marginID:
                    getValue;
                    break;
                case margin_bottomID:
                    getValue;
                    break;
                case margin_leftID:
                    getValue;
                    break;
                case margin_rightID:
                    getValue;
                    break;
                case margin_topID:
                    getValue;
                    break;
                case max_heightID:
                    getValue;
                    break;
                case max_widthID:
                    getValue;
                    break;
                case min_heightID:
                    getValue;
                    break;
                case min_widthID:
                    getValue;
                    break;
                case orphansID:
                    getValue;
                    break;
                case outlineID:
                    getValue;
                    break;
                case outline_colorID:
                    getValue;
                    break;
                case outline_styleID:
                    getValue;
                    break;
                case outline_widthID:
                    getValue;
                    break;
                case overflowID:
                    getValue;
                    break;
                case paddingID:
                    getValue;
                    break;
                case padding_bottomID:
                    getValue;
                    break;
                case padding_leftID:
                    getValue;
                    break;
                case padding_rightID:
                    getValue;
                    break;
                case padding_topID:
                    getValue;
                    break;
                case page_break_afterID:
                    getValue;
                    break;
                case page_break_beforeID:
                    getValue;
                    break;
                case page_break_insideID:
                    getValue;
                    break;
                case pauseID:
                    getValue;
                    break;
                case pause_afterID:
                    getValue;
                    break;
                case pause_beforeID:
                    getValue;
                    break;
                case pitchID:
                    getValue;
                    break;
                case pitch_rangeID:
                    getValue;
                    break;
                case play_duringID:
                    getValue;
                    break;
                case positionID:
                    getValue;
                    break;
                case quotesID:
                    getValue;
                    break;
                case richnessID:
                    getValue;
                    break;
                case rightID:
                    getValue;
                    break;
                case speakID:
                    getValue;
                    break;
                case speak_headerID:
                    getValue;
                    break;
                case speak_numeralID:
                    getValue;
                    break;
                case speak_punctuationID:
                    getValue;
                    break;
                case speech_rateID:
                    getValue;
                    break;
                case stressID:
                    getValue;
                    break;
                case table_layoutID:
                    getValue;
                    break;
                case text_alignID:
                    getValue;
                    break;
                case text_decorationID:
                    getValue;
                    break;
                case text_indentID:
                    getValue;
                    break;
                case text_transformID:
                    getValue;
                    break;
                case topID:
                    getValue;
                    break;
                case unicode_bidiID:
                    getValue;
                    break;
                case vertical_alignID:
                    getValue;
                    break;
                case visibilityID:
                    getValue;
                    break;
                case voice_familyID:
                    getValue;
                    break;
                case volumeID:
                    getValue;
                    break;
                case white_spaceID:
                    getValue;
                    break;
                case widowsID:
                    getValue;
                    break;
                case widthID:
                    getValue;
                    break;
                case word_spacingID:
                    getValue;
                    break;
                case z_indexID:
                    getValue;
                    break;
                default:
                    getValue;
                    break;
            } //end switch
            cssbuf[cssbuflen++].value = strdup(buf);
            debprintf("%s]",buf);
        }// end while((*pcur)&&(*pcur !='}'))
        cssEle_t * tmpcssElep = (cssEle_t*) malloc( cssbuflen * sizeof(cssEle_t));
        memcpy(tmpcssElep,cssbuf,cssbuflen*sizeof(cssEle_t));
        //cssID[cssIDs].len = cssbuflen;
        //cssID[cssIDs++].value = tmpcssElep;
        pcss[*cssS].len = cssbuflen;
        pcss[*cssS].value = tmpcssElep;
        *cssS = *cssS + 1;
        if(*pcur == '}')pcur ++;
    }//end while((*pcur))
    return 0;
}

void printcssID()
{
    int i;
    cssID_t *pcssID;
    for(i =0;i<cssIDs;i++){
        pcssID= &cssID[i];
        printf("IDname:%s(",pcssID->name);
        for( int j=0;j<pcssID->len;j++){
            printf("%d:%s;",((cssEle_t*)(pcssID -> value )) [j].id,((cssEle_t*)(pcssID -> value )) [j].value);
        }
        printf(")\n");
    }
}
cssAttr_t* getcssFromID(char * ID,char *tag =NULL)
{
    int i;
    cssID_t * pcssID;
    char buf[1024];
    if(tag){
        strcpy(buf,tag);
        strcat(buf,ID);
    }
    debprintf("look at %s",buf);
    for(i =0;i<cssIDs;i++){
        pcssID = &cssID[i];
        debprintf("search(%s)   ",pcssID -> name);
        if((strcmp(ID,pcssID-> name)== 0) || (strcmp(buf,pcssID-> name)== 0)) {
            debprintf("find %d \n",i);
            return ( (cssAttr_t*)&(pcssID -> len));        
        }
    }
    return NULL;
}
cssAttr_t* getcssFromClass(char * ID,char *tag =NULL)
{
    int i;
    cssID_t * pcssID;
    char buf[1024];
    if(!ID) return NULL;
    if(tag){
        strcpy(buf,tag);
        strcat(buf,ID);
    }
    debprintf("look at %s",buf);
    for(i =0;i<cssClasss;i++){
        pcssID = &cssClass[i];
        debprintf("search(%s)   ",pcssID -> name);
        if((strcmp(ID,pcssID-> name)== 0) || (strcmp(buf,pcssID-> name)== 0)) {
            debprintf("find %d \n",i);
            return ( (cssAttr_t*)&(pcssID -> len));        
        }
    }
    return NULL;
}
