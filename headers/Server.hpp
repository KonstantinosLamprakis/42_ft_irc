#ifndef SERVER_HPP
# define SERVER_HPP

# define BACKLOG 10
# define MAX_CONNECTIONS 1024 //max is amount of free ports (65535 - 1024 = 64511) without blocking the reserved ones (< 1024)
# define BUFFER_SIZE 512 // max string length - 1
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
# include <algorithm>
# include <cctype>

# include "Request.hpp"
# include "User.hpp"

const std::string SPACE = "\t\n ";

namespace Command {
	const std::string PASS = "PASS";
	const std::string NICK = "NICK";
	const std::string USER = "USER";
	const std::string JOIN = "JOIN";
	const std::string PRIVMSG = "PRIVMSG";
	const std::string NOTICE = "NOTICE";
	const std::string QUIT = "QUIT";
	// Operator's commands
	const std::string KICK = "KICK";
	const std::string INVITE = "INVITE";
	const std::string TOPIC = "TOPIC";
	const std::string MODE = "MODE";
};

class Server
{
	private:
		int							_port;
		std::string 				_password;
		int							_sockfd;
		struct	addrinfo*			_server_info;
		std::vector<struct pollfd>	_connection_fds;
		int							_amnt_connections;
		static bool					_signal_status;
		std::vector<User>			_users;
		std::vector<std::string>	_avlb_commands;

	public:
		Server();
		Server(int port, std::string password);

		void		start();
		Request 	parse(std::string input) const;
		void		execute(Request request, int user_index);
		void		listentosocket();
		void		close_and_free_socket(std::string err_msg);
		void		accept_connection();
		void		communicate(int i);
		static void	signal_handler(int signal);
		pollfd		init_pollfd();
		void		send_data(int n, std::string str);
		void		print_msg_to_user(std::string msg, int user_index);
		// TODO(KL) create also a func: print_msg_to_channel

		// commands
		void		pass(Request request, int user_index);
};

#endif