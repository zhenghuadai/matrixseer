/*
 * =====================================================================================
 *
 *       Filename:  smthBBS.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年05月07日 21时36分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *smthGetContentFromsTx(char *sTx,int lenSource)
{
    if(lenSource == 0) lenSource = strlen(sTx);
    char * tmp=(char*)malloc(lenSource*sizeof(char));
	char *p=sTx;
	/*
	if(!tmp)
	{
		printf("err\n");
		return NULL;
	};
   
	
	int i=0;
	while(*p!='\0')
	{
		if(*p=='<')
		{
			while((*p!='>')&&(*p!='\0'))p++;
			p++;
			continue;
		}
		tmp[i++]=*p;
		p++;
	}*/
    p=strstr(sTx,"prints");
	if(!p)
	{
		return NULL;
	}
	p+=8;
	int braces=1,i=0;
	while(braces)
	{//È¡³öprint()ÖÐµÄÄÚÈÝ
		/*
		if(*p==NULL)
			break;
		else
		{
			if(*p=='(')
				braces++;
			else
				if(*p==')')
				braces--;
		}*/
		switch(*p)
		{
		case '\0':goto outwhile;
		case '(':braces++;
			break;
		case ')':braces--;
			break;
		case '\\':if(*(p+1)=='n')
				  {
					  tmp[i++]='\n';
					  p+=2;
					  continue;
				  }
			     else
				 {
					 if(*(p+1)=='r')
					 {	
						 p+=2;
						continue;
					 }

				 }
			break;
		default:;

		}
        tmp[i++]=*p;
		p++;		
	}
outwhile:
	tmp[i-=2]='\0';
	return tmp;
}
