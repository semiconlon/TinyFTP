#include <string>
#include <iostream>
#include <cassert>

#include "Arguments.h"

int printArgumentsArgList(Arguments arg){
  int i = 0;
  std::string str = arg.getArgByIndex(i);

  while(str != ""){
    ++i;
    std::cout << str << "----";
    str = arg.getArgByIndex(i);
  }

  std:: cout << std::endl;

 return 0;
}

int main(){
  std::string str1, str2, str3, str4, str5, str6;
  str1 = "PWD";
  str2 = "USER test";
  str3 = "RENAME test test2";

  str4 = "PASS passsword ";
  str5 = " ";
  str6 = "";

  Arguments args1(str1), args2(str2), args3(str3), args4(str4), args5(str5),
            args6(str6);

  std:: cout << " result of args1 ";
  printArgumentsArgList(args1);

  std:: cout << " result of args2 ";
  printArgumentsArgList(args2);

  std:: cout << " result of args3 ";
  printArgumentsArgList(args3);

  std:: cout << " result of args4 ";
  printArgumentsArgList(args4);

  std:: cout << " result of args5 ";
  printArgumentsArgList(args5);

  std:: cout << " result of args6 ";
  printArgumentsArgList(args6);

  return 0;
}
