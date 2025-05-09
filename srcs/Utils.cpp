#include "../headers/Utils.hpp"

std::string to_uppercase(const std::string &str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

void remove_duplicates(std::vector<std::string> &vec){
    std::set<std::string> seen;
    for (unsigned long i = 0; i < vec.size(); i++) {
        std::string uppercase = to_uppercase(vec[i]);
        if (seen.find(uppercase) != seen.end()) {
            vec.erase(vec.begin() + i);
        } else {
            seen.insert(uppercase);
        }
    }
}

bool is_number(const std::string str) {
    if (str.empty()) return false;

    for (std::size_t i = 0; i < str.length(); i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}