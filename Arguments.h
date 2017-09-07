#ifndef ARGUMENTS_H
#define ARGUMENTS_H


#include <vector>
#include <string>
#include <cctype>

class Arguments{
public:
  Arguments(const std::string& sourceStr);

const std::string getArgByIndex(int index) const;
bool setArgByIndex(int index, const std::string& val);

private:
  bool split(std::string& source);
  std::string stringSplitBySpace(const std::string& source);
  std::vector<std::string> argsList_;
  std::string originalStr_;
};

#endif
