#include "../headers/Server.hpp"
Server::Server() {}

Server::Server() : _port(0), _password("") {}

Server::Server(Server &copy){
	// if server is ready -> fill the copy constructor
}

Server& Server::operator=(Server &old){
	//if server is ready -> fill the assignement operator
}

Server::~Server() {}

Server::Server(int port, std::string password){
    this->_port = port;
    this->_password = password;
}

void Server::start(){
    std::cout << "starting at port: " << this->_port << std::endl;
}

Request Server::parse(std::string input){
    std::cout << input;
    return Request();
}

void Server::execute(Request request){
    request = Request();
}