#ifndef __MAIL_T_HEAD__
#define __MAIL_T_HEAD__

class mail_t
{
    public:
    char* from;
    char* subject;
    char* to;
    char* date;
    int state;

    char* cc;
    char* content;
    char* storefn;
    char* mime;
    int attachs;
    int mid;/*inner id*/
    public:
    mail_t(){}
    mail_t(char* m){}
    public:
    void loadfromfile(FILE* fp){}
    void storetofile(FILE* fp){}
    
    void parsefromFile(char* fn);
    void parsefromMail(char* mail);
    int parseAbstractFromFile(char* fn);
    int parseAbstractFromMail(char* email);

    void close(){}
    ~mail_t(){}
};
#endif
