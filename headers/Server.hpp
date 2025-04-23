#ifndef SERVER_HPP
# define SERVER_HPP

# define BACKLOG 10
# define MAX_CONNECTIONS 1024 //max is amount of free ports (65535 - 1024 = 64511) without blocking the reserved ones (< 1024)
# include <string>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <exception>
# include <unistd.h>
# include <sys/fcntl.h>

# include "Request.hpp"
# include "User.hpp"

const std::string SPACE = "\t\n ";
const std::string CRLF = "\r\n";

namespace Command {
	const std::string PASS = "PASS";
	const std::string NICK = "NICK";
	const std::string USER = "USER";
	const std::string QUIT = "QUIT";
};

class Server
{
	private:
		int					_port;
		std::string 		_password;
		int					_sockfd;
		struct	addrinfo*	_server_info;
		int					_connection_fds[MAX_CONNECTIONS];

	public:
		Server();
		Server(int port, std::string password);
		Server(Server &copy);
		Server &operator=(Server &old);
		~Server();

		void	start();
		Request parse(std::string input) const;
		void	execute(Request request);
		void	listentosocket();
		void	run_connection(int fd);
		void	close_and_free_socket(std::string err_msg);
};


#endif