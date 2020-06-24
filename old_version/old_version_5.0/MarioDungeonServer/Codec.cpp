#include "Codec.h"
Codec::Codec()
{
	int ret=_Mysql.connect();
	if(ret==-1)
	{
		exit(1);
	}
}
string Codec::deCodeMessage(const string& recv)
{
	cout<<"recv: "<<recv<<endl;
	string requestFlag=recv.substr(0,3);
	string msg=recv.substr(3);
	cout<<"requestFlag: "<<requestFlag<<endl;
	cout<<"msg: "<<msg<<endl;
	if(requestFlag==REGISTER)
	{
        if(msg.size()==0)
        {
            std::cout<<"Register substr error"<<std::endl;
        }
        auto it = msg.find('\t');
        char name[20];
        strcpy(name,msg.substr(0,it).c_str());
        char password[30];
        strcpy(password,msg.substr(it).c_str());
		return _Mysql.Register(name,password);
	}
	else if(requestFlag==LOGIN)
	{
        if(msg.size()==0)
        {
            std::cout<<"LOGIN substr error"<<std::endl;
        }
        auto it = msg.find('\t');
        char name[20];
        strcpy(name,msg.substr(0,it).c_str());
        char password[30];
        strcpy(password,msg.substr(it).c_str());
		return _Mysql.Login(name,password);
	}
	else
	{
		return "";
	}
	
}
