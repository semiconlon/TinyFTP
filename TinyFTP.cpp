#include "FTPServer.h"


#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

// using namespace muduo;
// using namespace muduo::net;

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  muduo::net::EventLoop loop;
  muduo::net::InetAddress listenAddr(2121);
  FTPServer server(&loop, listenAddr, 4);
  server.start();
  loop.loop();
}
