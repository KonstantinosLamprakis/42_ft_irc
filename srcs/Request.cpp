#include "../headers/Request.hpp"

Request::Request(std::string const cmd, std::vector<std::string> const args): _cmd(cmd), _args(args){};

Request::Request(Request const &copy): _cmd(copy._cmd), _args(copy._args){}

Request &Request::operator=(Request const &old){
    this->_cmd = old._cmd;
    this->_args = old._args;
    return (*this);
}

Request::~Request(){}

std::string const Request::getCommand() const{
    return this->_cmd;
}

std::vector<std::string> const Request::get_args() const{
    return this->_args;
}