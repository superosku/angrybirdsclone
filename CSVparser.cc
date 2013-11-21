#include <string>
#include <vector>

#include "CSVparser.hh"

std::vector<std::string> toSTRVEC(std::string in)
{
  std::vector<std::string> out;
  size_t i = 0,j = 0;
  
  for (auto k : in)
  {
    if (k == ',')
    {
      out.push_back(in.substr(i,(j-i)));
      i = j+1;
    }
    j++;
  }
  out.push_back(in.substr(i,(j-i)));
  return(out);
}

std::string toCSVSTR(std::vector<std::string> in)
{
  std::string out;
  
  for (auto i : in)
  {
    out += i;
    out += ',';
  }
  out.pop_back();
  
  return (out);
}

