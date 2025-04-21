#include "../headers/Request.hpp"

Request::Request(std::string cmd, std::vector<std::string> args): _cmd(cmd), _args(args){};

Request::Request(Request &copy): _cmd(copy._cmd), _args(copy._args){}

Request& Request::operator=(Request &old){
    this->_cmd = old._cmd;
    this->_args = old._args;
    return (*this);
}

Request::~Request(){}

std::string const Request::getCommand() const{
    return this->_cmd;
}