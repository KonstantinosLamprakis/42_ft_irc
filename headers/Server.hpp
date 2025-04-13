#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <iostream>

class Server{
	private:
		int	_port;
		std::string _password; 
	public:
		Server();
		Server(int port, std::string password);
		void start();

};

#endif