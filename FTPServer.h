#ifndef FTPSERVER_H
#define FTPSERVER_H

#include <muduo/net/TcpServer.h>

#include <muduo/net/Callbacks.h>

#include "cmdDeal.h"
// RFC 959



class FTPServer
{
 public:
  FTPServer(muduo::net::EventLoop* loop,
             const muduo::net::InetAddress& listenAddr,
              const int & threadNum);

  void start();  // calls server_.start();

 private:
  void onConnection(const muduo::net::TcpConnectionPtr& conn);

  void onMessage(const muduo::net::TcpConnectionPtr& conn,
                 muduo::net::Buffer* buf,
                 muduo::Timestamp time);

  muduo::net::TcpServer server_;
};

#endif  // MUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H
