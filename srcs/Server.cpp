#include "../headers/Server.hpp"
#include "../headers/Exceptions.hpp"

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

/**
 * @brief starts server and initializes ports
 * 
 * @param sockfd fd of the connection bind to port
 * @param server_info input for getaddrinfo function - will contain "socket address structure"
 * @param server_hints input for getaddrinfo function - contains flags used to define server_info
 * @return "running connection"
 */

void Server::start(){
	struct	addrinfo	server_hints;

	memset(&server_hints, 0, sizeof(server_hints));
	server_hints.ai_family = AF_INET; //only IPv4 (later on we're only allowed to use functions for IPv4)
	server_hints.ai_socktype = SOCK_STREAM; //TCP makes sense for us (option would be UDP)
	server_hints.ai_flags = AI_PASSIVE; 

	if (getaddrinfo(NULL, std::to_string(this->_port).c_str(), &server_hints, &this->_server_info) != 0)
		throw ServerConnectionFailed();
	else
	{

	}

	std::cout << "starting at port: " << this->_port << std::endl;
	//freeaddrinfo(server_info);
}

Request Server::parse(std::string input){
	std::cout << input;
	return Request();
}

void Server::execute(Request request){
	request = Request();
}
