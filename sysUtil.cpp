#include "sysUtil.h"
#include <iostream>

int connect(uint32_t address, uint16_t port){
  struct sockaddr_in sin;
  int sockfd;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
      return -1;

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = address;
  sin.sin_port = htons(port);

  if (connect(sockfd, reinterpret_cast<struct sockaddr*>(&sin), sizeof(sin)) < 0){
    std::cout << "Error occur is connect function to " << address << " : " << port << std::endl;
    return -1;
  }

  return sockfd;
}

int passiveSocketBindListen(){
  struct sockaddr_in sin, sa;
  socklen_t sa_len = sizeof(sa);
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
      return -1;

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = 0;

  if (bind(sockfd, reinterpret_cast<struct sockaddr*>(&sin), sizeof(sin)) < 0)
      return -1;

  if (listen(sockfd, 5) < 0)
      return -1;

  getsockname(sockfd, reinterpret_cast<struct sockaddr *>(&sa), &sa_len);

  return sockfd;
}
