#include "MarioDungeonServer.h"

MarioDungeonServer::MarioDungeonServer(EventLoop *loop,const InetAddress& listenAddr):_server(loop,listenAddr,"MarioDungeonServer")
{
	_server.setConnectionCallback(std::bind(&Codec::onConnection,&_codec,_1));
	_server.setMessageCallback(std::bind(&MarioDungeonServer::onMessage,this,_1,_2,_3));
}
void MarioDungeonServer::onConnection(const TcpConnectionPtr& conn)
{
	
}
void MarioDungeonServer::onMessage(const TcpConnectionPtr& conn,Buffer* buf,Timestamp time)
{
	string msg(buf->retrieveAllAsString());
	cout<<"retrieveString: "<<msg<<endl;
	string responseMsg=_codec.deCodeMessage(conn,msg,time);
	cout<<"responseMsg:"<<responseMsg<<endl;
	if(responseMsg=="ChatRoomMessage")
		return;
	else
		conn->send(responseMsg);
}
void MarioDungeonServer::start()
{
	_server.start();
}
