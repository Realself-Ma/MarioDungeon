#include "Codec.h"
ConnectionSet conn_;
Codec::Codec()
{
	int ret=_Mysql.connect();
	if(ret==-1)
	{
		exit(1);
	}
}
void Codec::onConnection(const TcpConnectionPtr& conn)
{
	conn_.insert(conn);
}
string Codec::deCodeMessage(const TcpConnectionPtr& conn,const string& recv,Timestamp time)
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
		return _Mysql.Login(conn,name,password);
	}
	else if(rqstr==FLUSHROOMLIST)
	{
		char playerName[20];
        strcpy(playerName,msg.substr(0).c_str());
		//cout<<"FlushRoomList playerName:"<<playerName<<endl;
		return _Mysql.FlushRoomList(playerName);
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
		//cout<<"owner:"<<owner<<endl;
		return _Mysql.CreateRoom(roomName,owner);
	}
	else if(rqstr==DELETEROOM)
	{
        if(msg.size()==0)
        {
            std::cout<<"DELETEROOM substr error"<<std::endl;
        }
		char playerName[20];
        strcpy(playerName,msg.substr(0).c_str());
		return _Mysql.DeleteRoom(playerName);
	}
	else if(rqstr==ENTERROOM)
	{
		if(msg.size()==0)
        {
            std::cout<<"ENTERROOM substr error"<<std::endl;
        }
        auto it = msg.find('\t');
        char roomName[20];
        strcpy(roomName,msg.substr(0,it).c_str());
        char playerName[20];
        strcpy(playerName,msg.substr(it+1).c_str());
		_Mysql.RoomListnameMap_.insert(make_pair(conn,string(playerName)));
		return _Mysql.EnterRoom(roomName,playerName);
	}
	else if(rqstr==MESSAGE)
	{
		auto it=msg.find('\t');
		string playerName=msg.substr(0,it);
		_Mysql.ChatRoomnameMap_.insert(make_pair(conn,playerName));
		msg.erase(it,1);
		cout<<"MESSAGE:"<<msg<<endl;
		return _Mysql.ChatRoomMessage(conn,msg,playerName);	
	}
	else if(rqstr==LEAVEROOM)
	{
		char playerName[20];
        strcpy(playerName,msg.substr(0).c_str());
		return _Mysql.LeaveRoom(playerName);
	}
	else if(rqstr==STARTRQ)
	{
		auto it=msg.find('\t');
		char playerName[20];
		char roomName[50];
        strcpy(playerName,msg.substr(0,it).c_str());
        strcpy(roomName,msg.substr(it+1).c_str());
		return _Mysql.startRequest(conn,playerName,roomName);
	}
	else if(rqstr==READYRQ)
	{
		cout<<"ready msg:"<<msg<<endl;
		auto it1=msg.find_first_of('\t');
		auto it2=msg.find_last_of('\t');
		char playerName[20];
		char roomName[50];
		char ready_str[10];
		bool ready=false;
        strcpy(playerName,msg.substr(0,it1).c_str());
        strcpy(roomName,msg.substr(it1+1,it2-it1-1).c_str());
		strcpy(ready_str,msg.substr(it2+1).c_str());
		printf("%s\n",roomName);
		if(strcmp(ready_str,"ready")==0)
			ready=true;
		else if(strcmp(ready_str,"unready")==0)
			ready=false;
		else
			return "";
		return _Mysql.readyRequest(conn,playerName,roomName,ready);	
	}
	else if(rqstr==MAPRQ)
	{
		char playerName[20];
        strcpy(playerName,msg.substr(0).c_str());
		return _Mysql.mapRequest(conn,playerName);
	}
	else
	{
		return "";
	}
	
}
