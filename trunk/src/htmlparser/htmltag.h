#ifndef _HTML_TAG_HEADER_
#define _HTML_TAG_HEADER_
#define ATTRIB 0
#define VALUE 1
#define TAGEND 2
#define ATTRIBVALUE 3
#define TAGCOMPLETE 4

#define STARTTAG 0
#define ENDTAG 1

#define INITTAG {"html","/html","head","/head", "a","/a" ,"body","/body" ,"","br",\
"script","/script",	"p","/p",	"table","/table",	"tr","/tr",	"td","/td", 	\
"div","/div",		"","img",	"ul","/ul",		 "li","/li",	"font","/font",	\
"h1","/h1",		"h2","/h2",	"h3","/h3",		 "h4","/h4",	"h5","/h5",\
"style","/style",	"title","/title","","meta",		 "","link","","input",\
"b","/b",		"i","/i",	"form","/form",		  "center","/center","nobr","/nobr", \
"noscript","/noscript", "u","/u",	"strong","/strong",	"small","/small","iframe",  "/iframe",\
"span","/span" ,\
"","video",\
"undefinedid","/undefinedid", \
}

#define HTMLID 0
#define HEADID 2
#define ATAGID 4 
#define BODYID 6
#define BRID  9 
#define SCRIPTID 10
#define PTAGID  12
#define TABLEID 14 
#define TRID 16
#define TDID 18
#define DIVID 20
#define IMGID 23
#define ULID 24
#define LIID 26
#define FONTID 28
#define H1ID 30
#define H2ID 32
#define H3ID 34
#define H4ID 36
#define H5ID 38
#define STYLEID 40
#define TITLEID 42
#define METAID 45
#define LINKID 47
#define INPUTID 49
#define BTAGID 50
#define ITAGID 52
#define FORMID 54
#define CENTERID 56
#define NOBRID 58
#define NOSCRIPTID 60
#define UTAGID 62
#define STRONGID 64
#define SMALLID 66
#define IFRAMEID 68
#define SPANID 70
#define VEDIOID 73
#define MAXTAGID 73
#define VALUEOBJID 75
/*
#define Dprintf(a,...) \
{ \
#if DEB\
printf(a,...); \
#endif \
}
*/
//#define DEB
#define _AF2(a,b,c) tagfunc[a]=&htmlparser::b;tagfunc[a+1]=&htmlparser::c;
#define _AF1(a,b) tagfunc[a-1]=&htmlparser::SgetErrtag;tagfunc[a]=&htmlparser::b;
#define _ctAF1(a,b) ctTagFunc[a]=htmlparser::b;
#define _ctAF2(a,b,c) ctTagFunc[a]=htmlparser::b;ctTagFunc[a+1]=htmlparser::c;
#define _attrAF1(a,b) attrFunc[a]=&htmlparser::b;
#endif
