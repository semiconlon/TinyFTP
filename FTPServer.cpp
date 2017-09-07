#include <string>
#include <memory>
#include <map>
#include <functional>

#include "FTPServer.h"

#include <muduo/base/Logging.h>

#include <boost/bind.hpp>


#include "cmdDeal.h"


// using namespace muduo;
// using namespace muduo::net;

FTPServer::FTPServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr,
                       const int & threadNum)
  : server_(loop, listenAddr, "FTPServer")
{
  server_.setConnectionCallback(
      boost::bind(&FTPServer::onConnection, this, _1));
  server_.setMessageCallback(
      boost::bind(&FTPServer::onMessage, this, _1, _2, _3));
  server_.setThreadNum(threadNum);
}

void FTPServer::start()
{
  server_.start();
}

void FTPServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
  LOG_INFO << "FTPServer - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");

  conn->send("220 Service ready.\n");
}

void FTPServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
                           muduo::net::Buffer* buf,
                           muduo::Timestamp time)
{
  muduo::string msg(buf->retrieveAllAsString());
  LOG_INFO << conn->name() << " ftp: " << msg << " comming at " << time.toString();

  std::string args_msg(msg.c_str());
  Arguments args(args_msg);
  std::string cmd = args.getArgByIndex(0);
  if(find(g_cmds.begin(), g_cmds.end(), cmd) != g_cmds.end()){
    std::shared_ptr<Arguments> args_ptr(new Arguments(args));
    g_cmdFuncTable[cmd](conn, args_ptr);

  }
  else conn->send("Wrong Command!!!\n");

}
