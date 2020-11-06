#ifndef CODEC_H
#define CODEC_H

#include <string>
#include <vector>
#include "request.h"
#include "MysqlServer.h"
using namespace std;
class Codec
{
public:
	Codec();
	void onConnection(const TcpConnectionPtr& conn);
	string deCodeMessage(const TcpConnectionPtr& conn,const string& recv);
private:
	MysqlServer _Mysql;
};

#endif //CODEC_H
