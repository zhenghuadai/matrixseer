/**
 * JSDocument.h - Example for my tutorial : Scripting C++ with JavaScript
 * (c) 2002 - Franky Braem
 * http://www.braem17.yucom.be
 */

#ifndef _JSDocument_H
#define _JSDocument_H

#include "Document.h"

class JSDocument
{
public:
	/**
	 * Constructor
	 */
	JSDocument() : m_pDocument(NULL)
	{
	}
	
	/**
	 * Destructor
	 */
	virtual ~JSDocument()
	{
		delete m_pDocument;
		m_pDocument = NULL;
	}

	/**
	 * JSGetProperty - Callback for retrieving properties
	 */
	static JSBool JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

	/**
	 * JSSetProperty - Callback for setting properties
	 */
	static JSBool JSSetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

	/**
	 * JSConstructor - Callback for when a wxDocument object is created
	 */
	static JSBool JSConstructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	
	/**
	 * JSDestructor - Callback for when a wxDocument object is destroyed
	 */
	static void JSDestructor(JSContext *cx, JSObject *obj);

	/**
	 * JSInit - Create a prototype for wxDocument
	 */
	static JSObject* JSInit(JSContext *cx, JSObject *obj, JSObject *proto = NULL);

    static JSBool computeReduction(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    static JSBool Write(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

	static JSClass Document_class;

    void setDocument(Document *customer)
	{
		m_pDocument = customer; 
	}
    
	Document* getDocument() 
	{
		return m_pDocument; 
	}

protected:

private:

	Document *m_pDocument;

    static JSPropertySpec Document_properties[];
	static JSFunctionSpec Document_methods[];

	enum
	{
		name_prop,
		age_prop
	};
};

#endif //_JSDocument_H
