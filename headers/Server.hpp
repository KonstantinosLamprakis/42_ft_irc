#ifndef SERVER_HPP
# define SERVER_HPP

# define BACKLOG 10
# define MAX_CONNECTIONS 1024 //max is amount of free ports (65535 - 1024 = 64511) without blocking the reserved ones (< 1024)
# define BUFFER_SIZE 512 // rfc 2812
# include <string>
# include <cstring>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <exception>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>
# include <signal.h>
# include <sstream>


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
		int							_port;
		std::string 				_password;
		int							_sockfd;
		struct	addrinfo*			_server_info;
		// struct	pollfd			_connection_fds[MAX_CONNECTIONS];
		std::vector<struct pollfd>	_connection_fds;
		int							_amnt_connections;
		static bool					_signal_status;
		// std::vector<User>		_members;

		public:
		Server();
		Server(int port, std::string password);

		void		start();
		Request 	parse(std::string input) const;
		void		execute(Request request);
		void		listentosocket();
		void		close_and_free_socket(std::string err_msg);
		void		accept_connection();
		void		communicate(int i);
		static void	signal_handler(int signal);
		pollfd		init_pollfd();
		//void		send_data(Request in);
		//void		add_member();
};

#endif