#ifndef MYSQLSERVER_H
#define MYSQLSERVER_H

#include <iostream>
#include <cstring>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include "mysql/mysql.h"
#include "mysql/errmsg.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/base/Logging.h"
#include "request.h"
#include "MapGenerator.h"
using namespace std;
using namespace muduo;
using namespace muduo::net;
typedef std::map<TcpConnectionPtr,string> ConnectionMap;
typedef std::set<TcpConnectionPtr> ConnectionSet;
extern ConnectionSet conn_;
class MysqlServer
{
public:
	MysqlServer():mysql(nullptr),res_ptr(nullptr),sqlrow(0){}
	void brodcast(const string& msg);
	int connect();
	int real_connect();
	int sqlQuery(const char *query);
	string Register(char* name,char* password);
	string Login(const TcpConnectionPtr& conn,char* name,char* password);
	string FlushRoomList(char* playerName);
	string CreateRoom(char* roomName,char* owner);
	string DeleteRoom(char* playerName);
	string EnterRoom(char* roomName,char* playerName);
	string LeaveRoom(char *playerName);
	string ChatRoomMessage(const TcpConnectionPtr& conn,const string& playerName);
	string startRequest(const TcpConnectionPtr& conn,char* playername,char* roomName);	
	string readyRequest(const TcpConnectionPtr& conn,char* playername,char* roomName,bool ready);
	string mapRequest(const TcpConnectionPtr& conn,char* playername);
	string pkRequest(const TcpConnectionPtr& conn,char* playername);
	string pkPosUpdate(const TcpConnectionPtr& conn,char* playerName,char* x,char* y,char* dir);
	string pkStatusUpdate(const TcpConnectionPtr& conn,char* playerName,char* hp,char* atk,char* def);
	string fightRequest(const TcpConnectionPtr& conn,char* playerName);
	string gameoverRequest(const TcpConnectionPtr& conn,char* playerName);
	string initialReadyRequest(char* playerName);
	string initialPlayerRequest(char* playerName);
	string doOfflineRequest(char* playerName);

public:
	ConnectionMap RoomListnameMap_;
	ConnectionMap ChatRoomnameMap_;
	vector<string> msgList;
private:
	MYSQL *mysql;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
	MapGenerator MapGen_;
};

#endif //MYSQLSERVER_H
