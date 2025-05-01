#include <string>

std::string toUppercase(const std::string &str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}