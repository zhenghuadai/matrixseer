#ifndef Email_header
#define Email_header
class Email{
public:
	Email();
	int mailto(char *mailaddr,char *data,char *subject);
	int mailto2(char *mailaddr,char *data,char *subject,char *fn);
	int mailfrom(char *sender,char * passwd);
	int mailserver(char *server);
private:
	char * smtpserver;//="smtp.gmail.com";
	char * smtpsendaddr;//="djx.zhenghua@gmai.com";
	char * myaccount;//="djx.zhenghua@gmail.com";
	char * mypasswd;//="djx_zh010";

};
#endif
