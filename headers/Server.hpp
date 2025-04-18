#ifndef SERVER_HPP
# define SERVER_HPP

# define BACKLOG 10
# include <string>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <exception>

# include "Request.hpp"

class Server
{
	private:
		int					_port;
		std::string 		_password;
		int					_sockfd;
		struct	addrinfo*	_server_info;
		
	public:
		Server();
		Server(int port, std::string password);
		Server(Server &copy);
		Server& operator=(Server &old);
		~Server();

		void	start();
		Request	parse(std::string input);
		void	execute(Request request);
		void	listentosocket();
};

#endif