#ifndef UTILS_HPP
# define UTILS_HPP

#include <string>
#include <set>
#include <algorithm>
#include <cctype>
#include <vector>

const std::string SPACE = "\t\n ";

std::string toUppercase(const std::string &str);

void removeDuplicates(std::vector<std::string> &vec);

#endif