#include "MysqlServer.h"

int MysqlServer::connect()
{
	mysql = mysql_init(NULL);//数据库初始化
    if(!mysql)
    {
        LOG_ERROR<<"mysql init falied";
        return -1;
    }

    mysql = mysql_real_connect(mysql,SERVER_IP, "root", "140226", "MarioDungeon", 0, NULL, 0);//连接到数据库

    if(mysql)
    {
        LOG_INFO<<"MySQL connection success";
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
	int res=mysql_query(mysql, "set names utf8");//设置查询字符集为utf8
	if(res!=0)
	{
		LOG_INFO<<"mysql_query set utf8 error";
		return -1;
	}
    res=mysql_query(mysql,query);
    if(res)
    {
        return -1;
    }
    return 0;
}
string MysqlServer::Register(char* name,char* password)
{
	string strclientmsg="";
	char query[100];
	sprintf(query, "select username from UserInfo where username = '%s'", name);
	int ret;
	ret = sqlQuery(query);
	res_ptr = mysql_store_result(mysql); //即使不需要返回值也要这样，否则会出错
	sqlrow = mysql_fetch_row(res_ptr);
	if(sqlrow)
		strclientmsg += "The name has been registered";
	else
	{
		memset(query, 0, sizeof(query));
		sprintf(query, "insert into UserInfo values('%s', '%s',%d,%d)", name, password,0,0);
		ret = sqlQuery(query);
		if(ret == -1)
		{
			LOG_ERROR<<"insert error :"<<mysql_error(mysql);
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
			}
			//把自己加入到好友表中
			memset(query, 0, sizeof(query));
			sprintf(query, "insert into %s values('%s')", name, name);
			ret=sqlQuery(query);
			if(ret==-1)
			{
				LOG_ERROR<<"insert error :"<<mysql_error(mysql);
			}
			strclientmsg += "REGISTER SUCCESS";
			if(mysql_errno(mysql))
			{
				LOG_ERROR<<"retrive error :"<<mysql_error(mysql);
			}
		}
	}
	mysql_free_result(res_ptr);
	return strclientmsg;
}
string MysqlServer::Login(char* name,char* password)
{
	string strclientmsg="";
	char query[100];
	sprintf(query, "select password from UserInfo where username = '%s'",name);
	sqlQuery(query);
	res_ptr = mysql_store_result(mysql);
	sqlrow = mysql_fetch_row(res_ptr);
	if(!sqlrow)
		strclientmsg += "You have to register first";
	else
	{
		if(strcmp(sqlrow[0], password) == 0)
		{
			strclientmsg += "LOG IN SUCCESS";
			std::string namestr(name);
			mysql_free_result(res_ptr);
			
			memset(query,0,sizeof(query));
			sprintf(query,"update UserInfo set online=%d where username='%s'",1,name);
			sqlQuery(query);
		}
		else
			strclientmsg += "password is wrong";
		if(mysql_errno(mysql))
		{
			LOG_ERROR<<"Retrive error :"<<mysql_error(mysql);
		}
	}
	return strclientmsg;
}
