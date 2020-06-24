#ifndef MYSQLSERVER_H
#define MYSQLSERVER_H

#include <cstring>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include "mysql/mysql.h"
#include "muduo/base/Logging.h"
#include "request.h"
using namespace muduo;
class MysqlServer
{
public:
	MysqlServer():mysql(nullptr),res_ptr(nullptr),sqlrow(0){}
	int connect();
	int sqlQuery(const char *query);
	string Register(char* name,char* password);
	string Login(char* name,char* password);
public:
	MYSQL *mysql;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
};

#endif //MYSQLSERVER_H
