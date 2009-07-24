#ifndef _Document_H
#define _Document_H
using namespace std;
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
class Document
{
public:

    int GetAge() 
	{
		return m_buf; 
	}
    
	void SetAge(int newAge)
	{
		m_buf = newAge; 
	}
    
	std::string GetName() 
	{
		return m_name; 
	}
    
	void SetName(string newName) 
	{
		m_name = newName; 
	}
    void Write( char * a)
    {
        sprintf(phtml,"%s",a);
        phtml +=strlen(a);
    }
    void resetphtml()
    {
        phtml = html;
    }
    char html[1024*1024];
    typedef char * charp;
    charp  phtml;
 private:

    int  m_buf;
    string m_name;

};

#endif

