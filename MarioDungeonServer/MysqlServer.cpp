#include "MysqlServer.h"

int MysqlServer::connect()
{
	mysql = mysql_init(NULL);//数据库初始化
    if(!mysql)
    {
        LOG_ERROR<<"mysql init falied";
        return -1;
    }
	if(real_connect())
	{
        LOG_ERROR<<"mysql real_connect falied";
        return -1;		
	}
	
    return 0;
}
int MysqlServer::real_connect()
{
	mysql = mysql_real_connect(mysql,SERVER_IP, "root", "140226", "MarioDungeon", 0, NULL, 0);//连接到数据库

    if(mysql)
    {
        LOG_INFO<<"MySQL connection success";
		int res=mysql_query(mysql, "set names utf8");//设置查询字符集为utf8
		if(res!=0)
		{
			LOG_INFO<<"mysql_query set utf8 error";
			return -1;
		}
		else
			LOG_INFO<<"MySQL set utf8 success";
    }
    else
    {
        LOG_WARN<<" MySQL connection failed";
        return -1;
    }
	return 0;
}
int MysqlServer::sqlQuery(const char *query)
{
    int res=mysql_query(mysql,query);
    if(res)
    {
		int flag=mysql_errno(mysql);
		if(flag==CR_SERVER_LOST)
		{
			if(real_connect())
			{	
				LOG_ERROR<<"retry real_connect falied";
				return -1;		
			}
			else
			{
				LOG_INFO<<"retry real_connect success";
				res=mysql_query(mysql,query);
				if(res)
					return -1;
			}
		}
		else
			return -1;
    }
    return 0;
}
void MysqlServer::brodcast(const string& msg)
{
	for(auto it=conn_.begin();it!=conn_.end();++it)
			(*it)->send(msg);
}
string MysqlServer::Register(char* name,char* password)
{
	string Reply="";
	char query[100];
	sprintf(query, "select username from UserInfo where username = '%s'", name);
	int ret = sqlQuery(query);
	if(ret!=0)
		return Reply;
	res_ptr = mysql_store_result(mysql); //即使不需要返回值也要这样，否则会出错
	sqlrow = mysql_fetch_row(res_ptr);
	if(sqlrow)
		Reply += "The name has been registered";
	else
	{
		mysql_free_result(res_ptr);
		memset(query, 0, sizeof(query));
		sprintf(query, "insert into UserInfo values('%s', '%s',%d,%d)", name, password,0,0);
		ret = sqlQuery(query);
		if(ret == -1)
		{
			LOG_ERROR<<"insert error :"<<mysql_error(mysql);
			return Reply;
		}
		else
		{
			//注册成功
			//新建一个用户表，用于保存用户信息
			memset(query, 0, sizeof(query));
			sprintf(query, "create table %s (friends char(30));",name);
			ret=sqlQuery(query);
			if(ret==-1)
			{
				LOG_ERROR<<"create table error :"<<mysql_error(mysql);
				return Reply;
			}
			//把自己加入到好友表中
			memset(query, 0, sizeof(query));
			sprintf(query, "insert into %s values('%s')", name, name);
			ret=sqlQuery(query);
			if(ret==-1)
			{
				LOG_ERROR<<"insert error :"<<mysql_error(mysql);
				return Reply;
			}
			Reply += "REGISTER SUCCESS";
			if(mysql_errno(mysql))
			{
				LOG_ERROR<<"retrive error :"<<mysql_error(mysql);
				return Reply;
			}
		}
	}
	return Reply;
}
string MysqlServer::Login(const TcpConnectionPtr& conn,char* name,char* password)
{
	string Reply="";
	char query[100];
	sprintf(query, "select password from UserInfo where username = '%s'",name);
	int ret=sqlQuery(query);
	if(ret!=0)
		return Reply;
	res_ptr = mysql_store_result(mysql);
	sqlrow = mysql_fetch_row(res_ptr);
	if(!sqlrow)
		Reply += "You have to register first";
	else
	{
		if(strcmp(sqlrow[0], password) == 0)
		{
			Reply += "LOG IN SUCCESS";
			std::string namestr(name);
			mysql_free_result(res_ptr);
			memset(query,0,sizeof(query));
			sprintf(query,"update UserInfo set online=%d where username='%s'",1,name);
			sqlQuery(query);
		}
		else
			Reply += "password is wrong";
		if(mysql_errno(mysql))
		{
			LOG_ERROR<<"Retrive error :"<<mysql_error(mysql);
		}
	}

	return Reply;
}
string MysqlServer::FlushRoomList(char *playerName)
{
	string Reply="";
	char query[100];
	sprintf(query,"select name,owner from rooms");
	int ret=sqlQuery(query);
	if(ret!=0)
		return Reply;
	res_ptr=mysql_store_result(mysql);
	string allmsg;
	string name;
	string owner;
	int count=0;
	while((sqlrow=mysql_fetch_row(res_ptr)))
	{
		name+=sqlrow[0];
		owner+=sqlrow[1];
		//cout<<"RoomList name:"<<name<<endl;
		//cout<<"RoomList owner:"<<owner<<endl;
		allmsg=name+","+owner+",:";
		Reply+=allmsg;
		name.clear();
		owner.clear();
		allmsg.clear();
		count++;
	}
	std::cout<<"RoomsListReply: "<<Reply<<" count: "<<count<<std::endl;
	if(Reply==""&&count==0)
	{
		Reply+="rooms empty!";
	}
	else
	{
		char countstr[5];
		sprintf(countstr,"#%d#",count);
		Reply+=countstr;
	}
	mysql_free_result(res_ptr);
	return Reply;
}
string MysqlServer::CreateRoom(char* roomName,char* owner)
{
	//cout<<"CreateRoom roomName:"<<roomName<<endl;
	//cout<<"CreateRoom owner:"<<owner<<endl;
	string reply="";
	char query[100];
	sprintf(query,"select name from rooms where name='%s'",roomName);
	int ret=sqlQuery(query);
	if(ret!=0)
		return reply;
	res_ptr = mysql_store_result(mysql);
	sqlrow = mysql_fetch_row(res_ptr);
	if(sqlrow)
		reply+="duplicated name!";
	else
	{
		memset(query,0,sizeof(query));
		mysql_free_result(res_ptr);
		sprintf(query,"select owner from rooms where owner='%s'",owner);
		ret=sqlQuery(query);
		if(ret!=0)
			return reply;
		res_ptr = mysql_store_result(mysql);
		sqlrow = mysql_fetch_row(res_ptr);
		if(sqlrow)
			reply+="you already have a room!";
		else
		{
			memset(query,0,sizeof(query));
			mysql_free_result(res_ptr);
			sprintf(query,"insert into rooms values('%s','%s','%s',%d)",roomName,owner,"",0);
			ret=sqlQuery(query);
			if(ret==-1)
				LOG_ERROR<<"rooms insert error :"<<mysql_error(mysql);
			else
			{
				reply+="CreateRoom done!";
				brodcast("flush RoomList");
			}
		}
	}
	return reply;
}
string MysqlServer::DeleteRoom(char* playerName)
{
	char query[100];
	sprintf(query,"select name from rooms where owner='%s'",playerName);
	int ret=sqlQuery(query);
	if(ret!=0)
		return "";
	res_ptr = mysql_store_result(mysql);
	sqlrow = mysql_fetch_row(res_ptr);
	if(!sqlrow)
	{
		LOG_ERROR<<"Delete Room error!";
		return "No Room!";
	}
	string roomName=string(sqlrow[0]);
	memset(query,0,sizeof(query));
	mysql_free_result(res_ptr);
	sprintf(query,"delete from rooms where name='%s'",roomName.c_str());
	ret=sqlQuery(query);
	if(ret==-1)
	{
		LOG_ERROR<<"delete rooms error :"<<mysql_error(mysql);
		return "";
	}
	else
		brodcast("flush RoomList");
	return "";
}
string MysqlServer::EnterRoom(char* roomName,char* playerName)
{
	char query[100];
	sprintf(query,"select name from rooms where owner='%s'",playerName);
	int ret=sqlQuery(query);
	if(ret!=0)
		return "";
	res_ptr = mysql_store_result(mysql);
	sqlrow = mysql_fetch_row(res_ptr);
	bool haveRoom=false;
	if(sqlrow)
		haveRoom=true;
	memset(query,0,sizeof(query));
	mysql_free_result(res_ptr);
	sprintf(query,"select owner from rooms where name='%s'",roomName);
	ret=sqlQuery(query);
	if(ret)
		return "";
	res_ptr = mysql_store_result(mysql);
	sqlrow = mysql_fetch_row(res_ptr);
	if(!sqlrow)
	{
		LOG_ERROR<<"rooms find error!";
		return "";
	}
	else if(haveRoom&&strcmp(sqlrow[0],playerName)!=0)
	{
		return "Enter Refused!";
	}
	if(strcmp(sqlrow[0],playerName)!=0)
	{
		memset(query,0,sizeof(query));
		mysql_free_result(res_ptr);
		sprintf(query,"update rooms set player='%s' where name='%s'",playerName,roomName);
		ret=sqlQuery(query);
		if(ret==-1)
			LOG_ERROR<<"update rooms error :"<<mysql_error(mysql);
	}
	memset(query,0,sizeof(query));
	sprintf(query,"select owner,player from rooms where name='%s'",roomName);
	ret=sqlQuery(query);
	if(ret==-1)
	{
		LOG_ERROR<<"rooms name error :"<<mysql_error(mysql);
		return "";
	}
	res_ptr = mysql_store_result(mysql);
	sqlrow=mysql_fetch_row(res_ptr);
	string owner=string(sqlrow[0]);
	string player=string(sqlrow[1]);
	string re="EnterRoom"+owner+'\t'+player;
	cout<<"rooms EnterRoom:"<<re<<endl;
	for(int i=0;i<2;++i)
	{	
		if(string(sqlrow[i])!="")
		{
			for(auto it=RoomListnameMap_.begin();it!=RoomListnameMap_.end();++it)
			{
				if(it->second==sqlrow[i])
					(it->first)->send(re);
			}
		}
	}
	mysql_free_result(res_ptr);
	return "";
}
string MysqlServer::LeaveRoom(char *playerName)
{
	char query[100];
	sprintf(query,"select name from rooms where player='%s' or owner='%s'",playerName,playerName);
	int ret=sqlQuery(query);
	if(ret)
		return "";
	res_ptr = mysql_store_result(mysql);
	sqlrow = mysql_fetch_row(res_ptr);
	if(!sqlrow)
	{
		LOG_ERROR<<"roomName find error!";
		return "";
	}
	string roomName=string(sqlrow[0]);
	mysql_free_result(res_ptr);
	memset(query,0,sizeof(query));
	sprintf(query,"select owner,player from rooms where name='%s'",roomName.c_str());
	ret=sqlQuery(query);
	if(ret)
		return "";
	res_ptr = mysql_store_result(mysql);
	sqlrow = mysql_fetch_row(res_ptr);
	if(!sqlrow)
	{
		LOG_ERROR<<"owner,player find error!";
		return "";
	}
	string ownerName=string(sqlrow[0]);
	string re_RoomList="LeaveRoom"+string(playerName);
	string re_ChatRoom=string(playerName)+":Leave ChatRoom";
	int size=re_ChatRoom.size();
	char msgLen[10];
	snprintf(msgLen,sizeof(msgLen),"/%d/",size);
	string s(msgLen);
	re_ChatRoom=s+re_ChatRoom;
	for(int i=0;i<2;++i)
	{	
		if(string(sqlrow[i])!="")
		{
			for(auto it=RoomListnameMap_.begin();it!=RoomListnameMap_.end();++it)
			{
				cout<<"LeaveRoom Find: "<<re_RoomList<<endl;
				if(it->second==sqlrow[i])
					(it->first)->send(re_RoomList);
			}
			for(auto it=ChatRoomnameMap_.begin();it!=ChatRoomnameMap_.end();++it)
			{
				if(it->second==sqlrow[i])
					(it->first)->send(re_ChatRoom);
			}	
		}
	}
	//cout<<"LeaveRoom Msg: ownerName "<<ownerName<<" playerName:"<<playerName<<" roomName:"<<roomName<<endl;
	if(ownerName!=string(playerName))
	{
		memset(query,0,sizeof(query));
		mysql_free_result(res_ptr);
		sprintf(query,"update rooms set player='%s' where name='%s'","",roomName.c_str());
		//cout<<"LeaveRoom Msg:query "<<query<<endl;
		ret=sqlQuery(query);
		if(ret==-1)
			LOG_ERROR<<"update rooms error :"<<mysql_error(mysql);
	}
	return "";
}
string MysqlServer::ChatRoomMessage(const TcpConnectionPtr& conn,const string& playerName)
{
	char query[100];
	sprintf(query,"select name from rooms where owner='%s' or player='%s'",playerName.c_str(),playerName.c_str());
	int ret=sqlQuery(query);
	if(ret)
		return "";
	res_ptr = mysql_store_result(mysql);
	vector<char*> nameVec;
	vector<TcpConnectionPtr> connVec;
	while((sqlrow=mysql_fetch_row(res_ptr)))
	{
		nameVec.push_back(sqlrow[0]);
	}
	if(nameVec.size()==0)
	{
		LOG_ERROR<<"ChatRoomMessage error!";
		return "";
	}
	vector<string> strVec;
	for(int i=0;i<(int)nameVec.size();++i)
	{
		memset(query,0,sizeof(query));
		mysql_free_result(res_ptr);
		sprintf(query,"select owner,player from rooms where name='%s'",nameVec[i]);
		ret=sqlQuery(query);
		if(ret==-1)
		{
			LOG_ERROR<<"rooms name error :"<<mysql_error(mysql);
			return "";
		}
		res_ptr = mysql_store_result(mysql);
		sqlrow=mysql_fetch_row(res_ptr);
		strVec.push_back(string(sqlrow[0]));
		strVec.push_back(string(sqlrow[1]));
		for(int i=0;i<2;++i)
		{
			if(strVec[i]!="")
			{
				for(auto it=ChatRoomnameMap_.begin();it!=ChatRoomnameMap_.end();++it)
				{
					if(it->second==strVec[i])
						connVec.push_back(it->first);
				}
			}
		}
		strVec.clear();
	}
	char msgLen[10];
	int size;
	for(int i=0;i<(int)msgList.size();++i)
	{
		string msg=msgList[i];
		size=msg.size();
		memset(msgLen,0,sizeof(msgLen));
		snprintf(msgLen,sizeof(msgLen),"/%d/",size);
		string s(msgLen);
		msg=s+msg;
		cout<<"ChatRoomMessage:"<<msg<<endl;
		for(int i=0;i<(int)connVec.size();++i)
			connVec[i]->send(msg);
	}
	msgList.clear();
	return "ChatRoomMessage";
}
string MysqlServer::startRequest(const TcpConnectionPtr& conn,char* playername,char* roomName)
{	
	char query[100];
	sprintf(query,"select ready from rooms where name='%s'",roomName);
	int ret=sqlQuery(query);
	if(ret)
		return "";
	res_ptr = mysql_store_result(mysql);
	sqlrow = mysql_fetch_row(res_ptr);
	if(!sqlrow)
	{
		LOG_ERROR<<"ready find error!";
		return "";
	}
	string rq;	
	if(strcmp(sqlrow[0],"1")==0)
		rq="游戏开始!";
	else
		rq="玩家未准备!";
	msgList.push_back(rq);
	ChatRoomMessage(conn,playername);
	return "";
}
string MysqlServer::readyRequest(const TcpConnectionPtr& conn,char* playername,char* roomName,bool ready)
{
	cout<<"bool ready:"<<ready<<endl;
	int ready_num;
	if(ready)
		ready_num=1;
	else
		ready_num=0;
	char query[100];
	cout<<"ready_num:"<<ready_num<<endl;
	sprintf(query,"update rooms set ready=%d where name='%s'",ready_num,roomName);
 	int ret=sqlQuery(query);
	if(ret==-1)
		LOG_ERROR<<"update rooms error :"<<mysql_error(mysql);
	return "";
}
string MysqlServer::mapRequest(const TcpConnectionPtr& conn,char* playername)
{
	MapGen_.initialMap();
	string msg="mapStream";
	string floor_msg;
	for(int floor=0;floor<MapGen_.Total_Floor;++floor)
	{
		floor_msg.clear();
		for(int i=0;i<12;++i)
		{
			for(int j=0;j<16;++j)
			{
				floor_msg+=to_string(MapGen_.map[floor][i][j])+",";
			}
			floor_msg+=":";
		}
		floor_msg+="#";
		msg+=floor_msg;
	}
	msgList.push_back(msg);
	ChatRoomMessage(conn,playername);
	return "";
}
string MysqlServer::pkRequest(const TcpConnectionPtr& conn,char* playername)
{
	string msg="pkStart";
	msgList.push_back(msg);
	ChatRoomMessage(conn,playername);
	return "";
}
string MysqlServer::pkPosUpdate(const TcpConnectionPtr& conn,char* playerName,char* x,char* y,char* dir)
{
	string msg="pkPosUpdate";
	msg+=string(playerName)+"#"+string(x)+","+string(y)+","+string(dir);
	msgList.push_back(msg);
	ChatRoomMessage(conn,playerName);
	return "";
}
string MysqlServer::pkStatusUpdate(const TcpConnectionPtr& conn,char* playerName,char* hp,char* atk,char* def)
{
	string msg="pkStaUpdate";
	msg+=string(playerName)+"#"+string(hp)+","+string(atk)+","+string(def);
	msgList.push_back(msg);
	ChatRoomMessage(conn,playerName);
	return "";
}
string MysqlServer::fightRequest(const TcpConnectionPtr& conn,char* playerName)
{
	string msg="fightStart";
	msgList.push_back(msg);
	ChatRoomMessage(conn,playerName);
	return "";
}
string MysqlServer::gameoverRequest(const TcpConnectionPtr& conn,char* playerName)
{
	string msg="gameoverCall";
	msg+=string(playerName);
	msgList.push_back(msg);
	ChatRoomMessage(conn,playerName);
	return "";
}
string MysqlServer::initialReadyRequest(char* playerName)
{
	char query[100];
	snprintf(query,sizeof(query),"select name from rooms where player='%s'",playerName);
	int ret=sqlQuery(query);
	if(ret)
		return "";
	res_ptr = mysql_store_result(mysql);
	vector<char*> nameVec;
	while((sqlrow=mysql_fetch_row(res_ptr)))
	{
		nameVec.push_back(sqlrow[0]);
	}
	if(nameVec.size()==0)
	{
		LOG_ERROR<<"ChatRoomMessage error!";
		return "";
	}
	for(int i=0;i<(int)nameVec.size();++i)
	{
		memset(query,0,sizeof(query));
		mysql_free_result(res_ptr);
		snprintf(query,sizeof(query),"update rooms set ready=%d where name='%s'",0,nameVec[i]);
		ret=sqlQuery(query);
		if(ret==-1)
		{
			LOG_ERROR<<"update ready error :"<<mysql_error(mysql);
			return "";
		}
	}
	return "";
}
string MysqlServer::initialPlayerRequest(char* playerName)
{
	char query[100];
	snprintf(query,sizeof(query),"select name from rooms where player='%s'",playerName);
	int ret=sqlQuery(query);
	if(ret)
		return "";
	res_ptr = mysql_store_result(mysql);
	vector<char*> nameVec;
	while((sqlrow=mysql_fetch_row(res_ptr)))
	{
		nameVec.push_back(sqlrow[0]);
	}
	if(nameVec.size()==0)
	{
		LOG_ERROR<<"ChatRoomMessage error!";
		return "";
	}
	for(int i=0;i<(int)nameVec.size();++i)
	{
		memset(query,0,sizeof(query));
		mysql_free_result(res_ptr);
		snprintf(query,sizeof(query),"update rooms set player='%s' where name='%s'","",nameVec[i]);
		ret=sqlQuery(query);
		if(ret==-1)
		{
			LOG_ERROR<<"update player error :"<<mysql_error(mysql);
			return "";
		}
	}
	return "";
}
