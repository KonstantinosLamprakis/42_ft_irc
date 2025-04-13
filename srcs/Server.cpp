#include "../headers/Server.hpp"

Server::Server() : _port(0), _password("") {};

Server::Server(int port, std::string password){
    this->_port = port;
    this->_password = password;
};

void Server::start(){
    std::cout << "starting at port: " << this->_port << std::endl;
};