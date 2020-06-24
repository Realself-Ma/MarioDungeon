#ifndef CODEC_H
#define CODEC_H

#include <string>
#include "request.h"
#include "MysqlServer.h"
using namespace std;
class Codec
{
public:
	Codec();
	string deCodeMessage(const string& recv);
private:
	MysqlServer _Mysql;
};

#endif //CODEC_H
