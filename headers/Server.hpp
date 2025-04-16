#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>

# include "Request.hpp"

class Server{
	private:
		int	_port;
		std::string _password;
		
	public:
		Server();
		Server(int port, std::string password);
		Server(Server &copy);
		Server& operator=(Server &old);
		~Server();
		void start();
		Request parse(std::string input);
		void execute(Request request);
};

#endif