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
	else
	{
		return "";
	}
	
}
