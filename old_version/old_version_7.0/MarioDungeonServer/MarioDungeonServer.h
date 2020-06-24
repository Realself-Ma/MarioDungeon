#ifndef MARIODUNGEONSERVER_H
#define MARIODUNGEONSERVER_H

#include <cstdlib>
#include <set>
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/Logging.h"
#include "request.h"
#include "Codec.h"
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using namespace muduo;
using namespace muduo::net;
using muduo::string;
typedef std::set<TcpConnectionPtr> ConnectionSet;
class MarioDungeonServer
{
public:
	MarioDungeonServer(EventLoop *loop,const InetAddress& listenAddr);
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn,Buffer* buf,Timestamp time);
	void start();
private:
	TcpServer _server;
	Codec _codec;
	ConnectionSet _connections;
};


#endif // MARIODUNGEONSERVER_H
