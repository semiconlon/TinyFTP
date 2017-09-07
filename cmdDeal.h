#ifndef CMDDEAL_H
#define CMDDEAL_H

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <iostream>
#include <muduo/net/Callbacks.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Callbacks.h>

#include "sysUtil.h"
#include "Arguments.h"


//deal with the FTP command here by use the function table
//class muduo::net::TcpConnectionPtr;

using muduo::net::TcpConnectionPtr;

void doUSER(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doPASS(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doSYST(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doPWD(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doCWD(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doRNFR(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doRNTO(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doDELE(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doMKD(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doRMD(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doLIST(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doTYPE(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doPORT(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doPASV(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doRETR(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doSTOR(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);
void doQUIT(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args);

typedef std::shared_ptr<Arguments> ArgumentsPtr;
typedef std::function<void(muduo::net::TcpConnectionPtr, ArgumentsPtr)> Func;
typedef std::map<std::string,Func> MapFunc;

extern MapFunc g_cmdFuncTable;
extern std::vector<std::string> g_cmds;

//g_cmdFuncTable["USER"] = doUSER;
// g_cmdFuncTable["PASS"] = doPASS;
// g_cmdFuncTable["SYST"] = doSYST;
// g_cmdFuncTable["PWD"] = doPWD;
// g_cmdFuncTable["CWD"] = doCWD;
// g_cmdFuncTable["RNFR"] = doRNFR;
// g_cmdFuncTable["RNTO"] = doRNTO;
// g_cmdFuncTable["DELE"] = doDELE;
// g_cmdFuncTable["MKD"] = doMKD;
// g_cmdFuncTable["RMD"] = doRMD;
// g_cmdFuncTable["LIST"] = doLIST;
// g_cmdFuncTable["TYPE"] = doTYPE;
// g_cmdFuncTable["PORT"] = doPORT;
// g_cmdFuncTable["PASV"] = doPASV;
// g_cmdFuncTable["RETR"] = doRETR;
// g_cmdFuncTable["STORE"] = doSTORE;
// g_cmdFuncTable["QUIT"] = doQUIT;

#endif
