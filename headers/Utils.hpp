#ifndef UTILS_HPP
# define UTILS_HPP

#include <string>
#include <set>
#include <algorithm>
#include <cctype>
#include <vector>

const std::string SPACE = "\t\n ";

std::string to_uppercase(const std::string &str);

void remove_duplicates(std::vector<std::string> &vec);

#endif