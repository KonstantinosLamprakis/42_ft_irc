#include "../headers/Server.hpp"
#include <sstream>

Server::Server() : _port(0), _password("") {}

Server::Server(Server &copy): _port(copy._port), _password(copy._password){}

Server &Server::operator=(Server &old){
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

Request Server::_parse(std::string input) const {
    if (input.empty()) {
        throw std::invalid_argument("Input cannot be empty.");
    } else if (SPACE.find(input[0]) != std::string::npos) {
        throw std::invalid_argument("Input cannot start with a space.");
    } else if (input.size() >= 2 && input.substr(input.size() - 2) != CRLF) {
        throw std::invalid_argument("Input should end with \r\n characters.");
    }

    std::string command;
    std::vector<std::string> args;
    std::istringstream stream(input);
    std::string word;

    if (stream >> command) {
        args.push_back(command);
        while (stream >> word) {
            args.push_back(word);
        }
    }

    return Request(command, args);
}

void Server::_execute(Request request){
	if (request.getCommand() == Command::PASS) // TODO(KL)
        std::cout << "PASS" << std::endl;
	else
        throw std::invalid_argument("Invalid command.");
}