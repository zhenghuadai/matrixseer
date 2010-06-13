#ifndef Email_header
#define Email_header
class Email{
public:
	Email();
	int mailto(char *mailaddr,char *data,char *subject);
	int mailto2(char *mailaddr,char *data,char *subject,char *fn);
	int mailfrom(char *sender,char * passwd);
	int mailserver(char *server);
    void setAuth(char* server, char* user, char* pass);
    void setAuth(char* user, char* pass);
    ~Email();
private:
	char * smtpserver;
	char * smtpsendaddr;
	char * myaccount;
	char * mypasswd;

};
#endif
