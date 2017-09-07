#include "Arguments.h"

Arguments::Arguments(const std::string& sourceStr)
  :originalStr_(sourceStr)
{
  std::string spaceSplitStr = stringSplitBySpace(originalStr_);
  split(spaceSplitStr);
}

std::string Arguments::stringSplitBySpace(const std::string &source){
  std::string newStr;
  bool spaceFlag = true;

  for(auto beg = source.begin(), end = source.end(); beg != end; ++beg){
    if(!isspace(*beg)){
      newStr.push_back(*beg);
      spaceFlag = true;
    }
    else if(spaceFlag){
      newStr.append(" ");
      spaceFlag = false;
    }
  }

  if(isspace(*newStr.rbegin()))
  newStr.pop_back();

  return newStr;
}

bool Arguments::split(std::string &source){
  size_t pos = source.find(" ");
  size_t start = 0;

while(pos != std::string::npos){
  argsList_.push_back(source.substr(start, pos - start));
  start = pos + 1;
  pos = source.find(" ", start);
}

if(source.length() > 0)
  argsList_.push_back(source.substr(start));

return true;
}

const std::string Arguments::getArgByIndex(int index) const{
  //unsigned int uindex = static_cast<unsigned int>(index);
  if(static_cast<unsigned int>(index) < argsList_.size() &&  0 <= index)
  return argsList_[index];
  else
  return "";
}

bool Arguments::setArgByIndex(int index, const std::string& val){
  if(static_cast<unsigned int>(index) < argsList_.size() &&  0 <= index){
    argsList_[index] = val;
    return true;
  }
  else
  return false;
}
