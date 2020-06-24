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
	string rqstr=recv.substr(0,3);
	string msg=recv.substr(3);
	cout<<"rqstr: "<<rqstr<<endl;
	cout<<"msg: "<<msg<<endl;
	if(rqstr==REGISTER)
	{
        if(msg.size()==0)
        {
            std::cout<<"Register substr error"<<std::endl;
        }
        auto it = msg.find('\t');
        char name[20];
        strcpy(name,msg.substr(0,it).c_str());
        char password[30];
        strcpy(password,msg.substr(it+1).c_str());
		return _Mysql.Register(name,password);
	}
	else if(rqstr==LOGIN)
	{
        if(msg.size()==0)
        {
            std::cout<<"LOGIN substr error"<<std::endl;
        }
        auto it = msg.find('\t');
        char name[20];
        strcpy(name,msg.substr(0,it).c_str());
        char password[30];
        strcpy(password,msg.substr(it+1).c_str());
		return _Mysql.Login(name,password);
	}
	else if(rqstr==FLUSHROOMLIST)
	{
		return _Mysql.FlushRoomList();
	}
	else if(rqstr==CREATEROOM)
	{
        if(msg.size()==0)
        {
            std::cout<<"CREATEROOM substr error"<<std::endl;
        }
        auto it = msg.find('\t');
        char roomName[20];
        strcpy(roomName,msg.substr(0,it).c_str());
        char owner[20];
        strcpy(owner,msg.substr(it+1).c_str());
		cout<<"owner:"<<owner<<endl;
		return _Mysql.CreateRoom(roomName,owner);
	}
	else
	{
		return "";
	}
	
}
