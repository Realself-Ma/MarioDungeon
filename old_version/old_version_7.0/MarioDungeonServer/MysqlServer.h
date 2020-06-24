#ifndef MYSQLSERVER_H
#define MYSQLSERVER_H

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include "mysql/mysql.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/base/Logging.h"
#include "request.h"
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
	int sqlQuery(const char *query);
	string Register(char* name,char* password);
	string Login(const TcpConnectionPtr& conn,char* name,char* password);
	string FlushRoomList(char* playerName);
	string CreateRoom(char* roomName,char* owner);
	string DeleteRoom(char* playerName);
	string EnterRoom(char* roomName,char* playerName);
	string LeaveRoom(char *playerName);
	string ChatRoomMessage(const TcpConnectionPtr& conn,const string& msg,const string& playerName);	
public:
	ConnectionMap RoomListnameMap_;
	ConnectionMap ChatRoomnameMap_;
private:
	MYSQL *mysql;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
};

#endif //MYSQLSERVER_H
