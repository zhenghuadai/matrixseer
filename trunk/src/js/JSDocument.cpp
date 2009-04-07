/**
 * JSDocument.cpp - Example for my tutorial : Scripting C++ with JavaScript
 * (c) 2002 - Franky Braem
 * http://www.braem17.yucom.be
 */

#include <string>

#define XP_PC
#include <jsapi.h>

//#include "Document.h"
#include "JSDocument.h"
Document * gdocument = NULL;
JSPropertySpec JSDocument::Document_properties[] = 
{ 
    { "name", name_prop, JSPROP_ENUMERATE },
    { "age", age_prop, JSPROP_ENUMERATE },
    { 0 }
};

JSFunctionSpec JSDocument::Document_methods[] = 
{
    { "computeReduction", computeReduction, 1, 0, 0 },
    { "write", Write, 3, 0, 0 },
    { 0, 0, 0, 0, 0 }
};

JSClass JSDocument::Document_class = 
{ 
    "Document", JSCLASS_HAS_PRIVATE, JS_PropertyStub, JS_PropertyStub,
    JSDocument::JSGetProperty, JSDocument::JSSetProperty,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JSDocument::JSDestructor
};

JSBool JSDocument::JSGetProperty(JSContext *cx, JSObject *obj, jsval id,
        jsval *vp)
{
    if (JSVAL_IS_INT(id)) 
    {
        JSDocument *p = (JSDocument *) JS_GetPrivate(cx, obj);
        Document *customer = p->getDocument();
        switch (JSVAL_TO_INT(id)) 
        {
            case name_prop:
                {
                    std::string name = customer->GetName();
                    JSString *str = JS_NewStringCopyN(cx, name.c_str(), name.length());
                    *vp = STRING_TO_JSVAL(str);
                    break;
                }
            case age_prop:
                *vp = INT_TO_JSVAL(customer->GetAge());
                break;
        }
    }
    return JS_TRUE;
}

JSBool JSDocument::JSSetProperty(JSContext *cx, JSObject *obj, jsval id, 
        jsval *vp)
{
    if (JSVAL_IS_INT(id)) 
    {
        JSDocument *p = (JSDocument *) JS_GetPrivate(cx, obj);
        Document *customer = p->getDocument();

        switch (JSVAL_TO_INT(id)) 
        {
            case name_prop:
                {
                    JSString *str = JS_ValueToString(cx, *vp);
                    std::string name = JS_GetStringBytes(str);
                    customer->SetName(name);
                    break;
                }
            case age_prop:
                customer->SetAge(JSVAL_TO_INT(*vp));
                break;
        }
    }
    return JS_TRUE;
}

JSBool JSDocument::JSConstructor(JSContext *cx, JSObject *obj, uintN argc,
        jsval *argv, jsval *rval)
{
    JSDocument *priv = new JSDocument();
    Document* tmpdocument = new Document();
    if(!tmpdocument){
        printf("err \n");
        return JS_FALSE;
    }
    priv->setDocument(tmpdocument);
    JS_SetPrivate(cx, obj, (void *) priv);
    gdocument = tmpdocument;
    return JS_TRUE;
}

void JSDocument::JSDestructor(JSContext *cx, JSObject *obj)
{
    JSDocument *priv = (JSDocument*) JS_GetPrivate(cx, obj);
    delete priv;
    priv = NULL;
}

JSObject *JSDocument::JSInit(JSContext *cx, JSObject *obj, JSObject *proto)
{
    JSObject *newProtoObj = JS_InitClass(cx, obj, proto, &Document_class, 
            JSDocument::JSConstructor, 0,
            NULL, JSDocument::Document_methods,
            NULL, NULL);
    JS_DefineProperties(cx, newProtoObj, JSDocument::Document_properties);
    return newProtoObj;
}

JSBool JSDocument::computeReduction(JSContext *cx, JSObject *obj, uintN argc,
        jsval *argv, jsval *rval)
{
    JSDocument *p = (JSDocument*) JS_GetPrivate(cx, obj);
    if ( p->getDocument()->GetAge() < 25 )
        *rval = INT_TO_JSVAL(10);
    else
        *rval = INT_TO_JSVAL(5);
    return JS_TRUE;
}


JSBool JSDocument::Write(JSContext *cx, JSObject *obj, uintN argc,
        jsval *argv, jsval *rval)
{
    JSDocument *p = (JSDocument*) JS_GetPrivate(cx, obj);
    JSString *str = JS_ValueToString(cx, argv[0]);
    char * ppp = JS_GetStringBytes(str);
    p -> getDocument() -> Write(ppp);

    //       printf("%d,%s exe it\n",argc,ppp);
    return JS_TRUE;
}

