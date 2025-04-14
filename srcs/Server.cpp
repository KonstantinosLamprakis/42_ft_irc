#include "../headers/Server.hpp"

Server::Server() : _port(0), _password("") {};

Server::Server(int port, std::string password){
    this->_port = port;
    this->_password = password;
};

void Server::start(){
    std::cout << "starting at port: " << this->_port << std::endl;
};

/**
 * @brief parse the user input, returns a request and throws an exception in case of error
 * 
 * @param input the user raw input
 * @return Request the request to be executed, contains the command and the rest of the args
 */
Request Server::parse(std::string input){
    // TODO(KL): implement this
    std::cout << input;
    return Request();
};

/**
 * @brief execute the command which is contained in the request and throws an exception in case of error
 * 
 * @param request the request which contains the command to be executed
 */
void Server::execute(Request request){
    // TODO(KL): implement this
    request = Request();
};