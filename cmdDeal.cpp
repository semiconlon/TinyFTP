#include "cmdDeal.h"

static std::string g_oldName;
static int g_dataFd = -1;
static bool g_passive =  false;

MapFunc g_cmdFuncTable ={
                          {"USER", doUSER}, {"PASS", doPASS}, {"SYST", doSYST}, {"PWD", doPWD},
                          {"CWD", doCWD}, {"RNFR", doRNFR}, {"RNTO", doRNTO}, {"DELE", doDELE},
                          {"MKD", doMKD}, {"RMD", doRMD}, {"LIST", doLIST}, {"TYPE", doTYPE},
                          {"PORT", doPORT}, {"PASV", doPASV}, {"RETR", doRETR}, {"STOR", doSTOR},
                          {"QUIT", doQUIT}
                        };

std::vector<std::string> g_cmds = {
                                    "USER", "PASS", "SYST", "PWD", "CWD", "RNFR",
                                    "RNTO", "DELE", "MKD", "RMD", "LIST", "TYPE",
                                    "PORT", "PASV", "RETR", "STOR", "QUIT"
                                  };

//class boost::shared_ptr<muduo::net::TcpConnection> ;

void doUSER(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string username = args->getArgByIndex(1);
  std::string output;
  std::cout << "USER : " << username << std::endl;
  if(username == "test"){
    output = "331 User name ok, need password.\n";
    std::cout << "Right username !!!" << std::endl;
  }
  else{
    output = "332 Need account for login.\n";
    std::cout << "Wrong username !!!" << std::endl;
  }

  conn->send(output);
}

void doPASS(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string password = args->getArgByIndex(1);
  std::string output;
  std::cout << "PASS : " << password << std::endl;

  if(password == "1234"){
    output = "230 User logged in, proceed.\n";
    std::cout << "Right Password !!!" << std::endl;
  }
  else{
    output = "530 Not logged in.\n";
    std::cout << "Wrong Password !!!" << std::endl;
  }

  conn->send(output);
}

void doSYST(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::cout << "SYST : SHOW" << std::endl;
  std::string output("215 UNIX Type: L8.\n");

  conn->send(output);
}

void doPWD(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::cout << "SYST : PWD" << std::endl;
  char buf[512];
  if(getcwd(buf, sizeof(buf)) != NULL){
    std::string pathname(buf);
    std::cout << "Current path is " << pathname << std::endl;
    std::string output("257 " + pathname + "\n");

    conn->send(output);
  }
}

void doCWD(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string newDirctory = args->getArgByIndex(1);
  std::cout << "CWD : " <<  newDirctory << std::endl;
  char buf[512];

  if(getcwd(buf, sizeof(buf)) != NULL){
    strcat(buf, "/");
    strcat(buf, newDirctory.c_str());
    std::string pathname(buf);
    std::cout << "Change to path : "<< pathname << std::endl;
    std::string output;
    if(chdir(buf) < 0)
      output = "550 Failed to change directory.\n";
    else
      output = "250 Directory successfully changed.\n";

    conn->send(output);
  }

}

void doRNFR(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string oldName = args->getArgByIndex(1);
  g_oldName = oldName;
  std::cout << "RNFR : " <<  oldName << std::endl;
  std::string output("350 Requested file action pending further information.\n");

  conn->send(output);
}

void doRNTO(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string newName = args->getArgByIndex(1);
  std::cout << "RNTO : " << newName << std::endl;
  std::string output;
  if(rename(g_oldName.c_str(), newName.c_str()) < 0)
    output = "550 Failed to rename file.\n";
  else
    output = "250 File successfully renamed.\n";

  g_oldName = "";
  conn->send(output);
}

void doDELE(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string deleteFile = args->getArgByIndex(1);
  std::cout << "DELE : " << deleteFile << std::endl;

  std::string output;
  if(remove(deleteFile.c_str()) < 0)
    output = "550 Failed to remove file.\n";
  else
    output = "250 File successfully removed.\n";

  conn->send(output);
}

void doMKD(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string newDirctory = args->getArgByIndex(1);
  std::cout << "MKD : " << newDirctory << std::endl;

  std::string output;
  if(mkdir(newDirctory.c_str(), 0755) < 0)
    output = "550 Failed to create directory.\n";
  else
    output = "257 Directory successfully created.\n";

  conn->send(output);
}

void doRMD(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string deleteDirctory = args->getArgByIndex(1);
  std::cout << "RMD : " << deleteDirctory << std::endl;

  std::string output;
  if(rmdir(deleteDirctory.c_str()) < 0)
    output = "550 Failed to create directory.\n";
  else
    output = "250 Directory successfully removed.\n";

  conn->send(output);
}

void doLIST(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::cout << "LIST : " << std::endl;
  std::string output = "125 Data connection already open; transfer starting\n";
  conn->send(output);

  struct sockaddr_in sa;
  socklen_t sa_len = sizeof(sa);
  char buffer[512];
  std::string strBuf;
  std::string ls = "ls -l 2>&1";

  FILE* file = popen(ls.c_str(), "r");
  if (!file){
      output = "450 Requested file action not taken. File unavaible.\n";
      conn->send(output);
      close(g_dataFd);
      g_dataFd = -1;
  }
  else
  {
       if (g_passive)
          g_dataFd = accept(g_dataFd, reinterpret_cast<struct sockaddr *>(&sa), &sa_len);

      while (!feof(file))
          if (fgets(buffer, 512, file) != NULL)
              strBuf.append(buffer);
      std::cout << "The ls result is " << strBuf << std::endl;

      send(g_dataFd, strBuf.c_str(), strBuf.size(), 0);
      close(g_dataFd);

      pclose(file);
      output = "250 Closing data connection. Requested file action successful.\n";
      conn->send(output);
      g_dataFd = -1;
  }
  conn->send(output);

}

void doTYPE(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string newMode = args->getArgByIndex(1);
  std::cout << "TYPE : " << newMode << std::endl;

  std::string output;
  if (!strcmp(newMode.c_str(), "A"))
    output = "200 Switching to ASCII mode.\n";
  else if (!strcmp(newMode.c_str(),"I"))
    output = "200 Switching to Binary mode.\n";
  else if (!strcmp(newMode.c_str(),"L"))
    output = "200 Switching to Tenex mode.\n";
  else
    output = "501 Syntax error in parameters or arguments.\n";

  conn->send(output);
}

void doPORT(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string ipAndPort= args->getArgByIndex(1);
  std::cout << "IP PORT : " << ipAndPort << std::endl;
  std::vector<uint16_t> nums;
  size_t start = 0;
  size_t pos = ipAndPort.find(",");
  uint16_t temp;
  while(pos != std::string::npos){
    temp = static_cast<uint16_t>(atoi(ipAndPort.substr(start, pos - start).c_str()));
    nums.push_back(temp);
    start = pos + 1;
    pos = ipAndPort.find(",",start);
  }

  temp = static_cast<uint16_t>(atoi(ipAndPort.substr(start).c_str()));
  nums.push_back(temp);

 /*
    std::streamstring ss(ipAndPort);
    int temp;
    while(ip >> temp)
      nums.push_back(temp);
  */
  uint32_t ip = static_cast<uint32_t>(nums[3]) << 24 | static_cast<uint32_t>(nums[2]) << 16 \
                | static_cast<uint32_t>(nums[1]) << 8 | static_cast<uint32_t>(nums[0]);
  uint16_t port = nums[4] << 8 | nums[5];
  std::cout << "PORT : " << port << std::endl;


  std::string output = "200 Okey\n";
  g_passive = false;
  if((g_dataFd = connect(ip, port) ) < 0){
    output = "500 Cannot get the data socket\n";
    g_dataFd = -1;
    g_passive = false;
    std::cout << "Unable to connect to " << ip << " : " << port << std::endl;
  }

  conn->send(output);
}

void doPASV(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  g_passive = true;
  g_dataFd = passiveSocketBindListen();
  std::string  output = "227 Entering Passive Mode in server-end.\n";
  if(g_dataFd < 0){
    g_passive =  false;
    g_dataFd = -1;
    std::cout << "Unable to change to passive mode" << std::endl;
  }

  conn->send(output);
}

void doRETR(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string filename = args->getArgByIndex(1);
  std::cout << "RETR : " << filename << std::endl;

  std::string output;
  FILE* file = fopen(filename.c_str(), "rb");
  if (!file){
      output = "450 Requested file action not taken. File unavaible.\n";
      conn->send(output);
      close(g_dataFd);
      g_dataFd = -1;
  }
  else
  {
      output = "150 File status okay; about to open data connection.\n";
      conn->send(output);
      struct sockaddr_in sa;
      socklen_t sa_len = sizeof(sa);
      char buffer[512];
      size_t n;
      if (g_passive)
          g_dataFd = accept(g_dataFd,reinterpret_cast<struct sockaddr *>(&sa), &sa_len);
      do{
          n = fread(buffer, sizeof(char), 512, file);
          send(g_dataFd, buffer, n, 0);

      } while (n == 512);

      output = "226 Closing data connection. Requested file action successful.\n";
      conn->send(output);
      fclose(file);
      close(g_dataFd);
      g_dataFd = -1;
 }
}

void doSTOR(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::string filename = args->getArgByIndex(1);
  std::cout << "STOR : " << filename << std::endl;

  std::string output;
  FILE* file = fopen(filename.c_str(), "wb");
  if (!file){
      output = "450 Requested file action not taken. File unavaible.\n";
      conn->send(output);
      close(g_dataFd);
      g_dataFd = -1;
  }
  else
  {
      output = "150 File status okay; about to open data connection.\n";
      conn->send(output);

      struct sockaddr_in sa;
      socklen_t sa_len = sizeof(sa);
      char buffer[512];
      int n;
      if (g_passive)
          g_dataFd = accept(g_dataFd,reinterpret_cast<struct sockaddr *>(&sa), &sa_len);
      do{
          n = static_cast<int>(recv(g_dataFd, buffer, 512, 0));
          fwrite(buffer, sizeof(char) , n, file);

      } while (n == 512);

      output = "226 Closing data connection. Requested file action successful.\n";
      conn->send(output);
      fclose(file);
      close(g_dataFd);
      g_dataFd = -1;
}

}

void doQUIT(const muduo::net::TcpConnectionPtr& conn, std::shared_ptr<Arguments> args){
  std::cout << "QUIT : " << std::endl;
  std::string output = "221 Service closing control connection.\n";

  conn->send(output);
  close(g_dataFd);
  g_dataFd = -1;
  conn->shutdown();
}
