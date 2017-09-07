#include <string>
#include "../Arguments.h"


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


  return 0;
}
