#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <iostream>

# include "Request.hpp"

class Server{
	private:
		int	_port;
		std::string _password; 
	public:
		Server();
		Server(int port, std::string password);
		void start();
		Request parse(std::string input);
		void execute(Request request);
};

#endif