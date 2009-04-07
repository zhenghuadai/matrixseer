#include <stdio.h>
#include <stdlib.h>
#include "html.h"
#include "jsapi.h"
#include "JSDocument.h"
#include "dmfile.h"
char *httpsock(char * url,int *errcode);
JSRuntime *rt;
JSContext *cx;
JSObject *globalObj;
JSClass globalClass =
{
    "Global", 0,
    JS_PropertyStub,  JS_PropertyStub,JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub,  JS_FinalizeStub
};
int initJS()
{
    rt = JS_Init(1000000L);
    if ( !rt) 
        return 1;
    cx = JS_NewContext(rt, 8192);
    if ( !cx )
        return 1;
    globalObj = JS_NewObject(cx, &globalClass, 0, 0);
    if ( !globalObj )
        return 1;
    printf("init js\n");
    JS_InitStandardClasses(cx, globalObj);
        JSDocument::JSInit(cx, globalObj);
    printf("init js end\n");
    return 0;
}
int destroyJS()
{
    JS_DestroyContext(cx);
    JS_Finish(rt);
    return 0;
}

int testJS()
{
    jsval retval;
    JSString *str;
    char *myscript0 = "var c = new Customer();\
                       c.name = \"Franky\";\
                       c.age = 32;\
                       c.computeReduction();";
    char * myscript = " var document = new Document(); ";  
    uintN lineno=0;
    JSBool ok ;
    ok= JS_EvaluateScript(cx,globalObj,myscript,strlen(myscript),"abc",lineno,&retval);
    if(ok == JS_TRUE)
    {
        str = JS_ValueToString(cx,retval);
        char *s =JS_GetStringBytes(str);
        printf("result:%s\n",s);
    }
}
int main(int argc ,char *argv[])
{
    char *buf;
    FILE* fp;
    int ret;
    char * url;
    ret =   initJS();
    if(ret){
        printf("error");
        exit(0);
    }
    testJS();

    if(argc ==1) 
        url = "www.google.com";
    else
        url = argv[1];
    if(isfile(url)){
        buf = getsfromfile(url);
        if(buf == 0) {
            exit(0);
        }
    }
    else
        buf=httpsock(url,(int*)NULL);
        /*
    fp=fopen(argv[1],"r");
    buf=(char*)(malloc(10*1024*1024));
    int i=0;
    char c;
    c=fgetc(fp);
    while(c!=EOF)
    {
        buf[i++]=c;
        c=fgetc(fp);
    }
    buf[i]=0;
    */
    htmlparser hp;
    hp.parsehtml(buf);
    hp.renderhtml();
    printf("\n");
    hp.destroy();
    return 1;
}
