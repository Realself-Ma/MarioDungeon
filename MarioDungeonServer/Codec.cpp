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
string Codec::deCodeMessage(const TcpConnectionPtr& conn,const string& recv)
{
	if(recv.size()<3)
		return "";
	//cout<<"recv: "<<recv<<endl;
	string rqstr=recv.substr(0,3);
	string msg=recv.substr(3);
	//cout<<"rqstr: "<<rqstr<<endl;
	//cout<<"msg: "<<msg<<endl;
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
		_Mysql.msgList.push_back(msg);
		return _Mysql.ChatRoomMessage(conn,playerName);	
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
	else if(rqstr==PKRQ)
	{
		char playerName[20];
        strcpy(playerName,msg.substr(0).c_str());
		return _Mysql.pkRequest(conn,playerName);
	}
	else if(rqstr==UDPKPOSRQ)
	{
		auto pos_it1=msg.find_first_of('\t');
		auto pos_it2=msg.find('#');
		auto pos_it3=msg.find_last_of('\t');
		char playerName[20];
		char x[4];
		char y[4];
		char dir[4];
		snprintf(playerName,sizeof(playerName),msg.substr(0,pos_it1).c_str());//snprintf比strcpy更加安全，不会出现缓冲区溢出
		snprintf(x,sizeof(x),msg.substr(pos_it1+1,pos_it2-pos_it1-1).c_str());
		snprintf(y,sizeof(y),msg.substr(pos_it2+1,pos_it3-pos_it2-1).c_str());
		snprintf(dir,sizeof(dir),msg.substr(pos_it3+1).c_str());
		return _Mysql.pkPosUpdate(conn,playerName,x,y,dir);
	}
	else if(rqstr==UDPKSTATUSRQ)
	{
		auto sta_it1=msg.find_first_of('\t');
		auto sta_it2=msg.find('#');
		auto sta_it3=msg.find_last_of('\t');
		char playerName[20];
		char hp[8];
		char atk[8];
		char def[8];
		snprintf(playerName,sizeof(playerName),msg.substr(0,sta_it1).c_str());//snprintf比strcpy更加安全，不会出现缓冲区溢出
		snprintf(hp,sizeof(hp),msg.substr(sta_it1+1,sta_it2-sta_it1-1).c_str());
		snprintf(atk,sizeof(atk),msg.substr(sta_it2+1,sta_it3-sta_it2-1).c_str());
		snprintf(def,sizeof(def),msg.substr(sta_it3+1).c_str());
		return _Mysql.pkStatusUpdate(conn,playerName,hp,atk,def);
	}
	else if(rqstr==FIGHTRQ)
	{
		char playerName[20];
        snprintf(playerName,sizeof(playerName),msg.substr(0).c_str());
		return _Mysql.fightRequest(conn,playerName);
	}
	else if(rqstr==GAMEOVERRQ)
	{
		char playerName[20];
        snprintf(playerName,sizeof(playerName),msg.substr(0).c_str());
		return _Mysql.gameoverRequest(conn,playerName);
	}
	else if(rqstr==INITREADYRQ)
	{
		char playerName[20];
        snprintf(playerName,sizeof(playerName),msg.substr(0).c_str());
		return _Mysql.initialReadyRequest(playerName);
	}
	else if(rqstr==INITPLAYERRQ)
	{
		cout<<"INITPLAYERRRQ"<<endl;
		char playerName[20];
        snprintf(playerName,sizeof(playerName),msg.substr(0).c_str());
		return _Mysql.initialPlayerRequest(playerName);
	}
	else if(rqstr==OFFLINERQ)
	{
		char playerName[20];
        snprintf(playerName,sizeof(playerName),msg.substr(0).c_str());
		return _Mysql.doOfflineRequest(playerName);
	}
	else if(rqstr==TRUNCATETABLERQ)
	{
		return _Mysql.TruncateTableRequest();
	}
	else
	{
		return "";
	}
	
}
