#include "../headers/Utils.hpp"

std::string toUppercase(const std::string &str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

void removeDuplicates(std::vector<std::string> &vec){
    std::set<std::string> seen;
    for (unsigned long i = 0; i < vec.size(); i++) {
        std::string uppercase = toUppercase(vec[i]);
        if (seen.find(uppercase) != seen.end()) {
            vec.erase(vec.begin() + i);
        } else {
            seen.insert(uppercase);
        }
    }
}