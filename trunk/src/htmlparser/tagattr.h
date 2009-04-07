#ifndef ATTR_HEAD
#define ATTR_HEAD
#define INITATTR {"color","name","class","style","method","action","id","type","value","src",\
                  "href","background","title","onclick","submit","align","cols","rows"}
#define color_attr_id 0
#define name_attr_id  1
#define class_attr_id 2
#define style_attr_id 3
#define method_attr_id 4
#define action_attr_id 5 
#define id_attr_id     6
#define type_attr_id  7
#define value_attr_id  8
#define src_attr_id  9
#define href_attr_id  10
#define background_attr_id 11 
#define title_attr_id  12
#define onclick_attr_id  13
#define submit_attr_id  14
#define align_attr_id  15
#define cols_attr_id  16
#define rows_attr_id  17
typedef struct
{
	int id;
	union
	{
	int  v;
	char *p;
	}value;
	#define attrsofthistag  value.v
}tagAttr_t;
#endif
