#include "../headers/Server.hpp"

Server::Server() : _port(0), _password("") {}

Server::Server(Server &copy): _port(copy._port), _password(copy._password){}

Server& Server::operator=(Server &old){
    this->_port = old._port;
    this->_password = old._password;    
    return (*this);
}

Server::~Server() {}

Server::Server(int port, std::string password){
    this->_port = port;
    this->_password = password;
}

void Server::start(){
    std::cout << "starting at port: " << this->_port << std::endl;
}

Request Server::parse(std::string input) {
    std::cout << input;
    std::vector<std::string> args;
    args.push_back("test"); 
    Request req("command", args); 
    return req;
}

void Server::execute(Request request){
    std::cout << request.getCommand() << std::endl;
}