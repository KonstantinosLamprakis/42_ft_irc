#ifndef SERVER_HPP
# define SERVER_HPP

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
# include <map>

# include "Exceptions.hpp"
# include "Utils.hpp"
# include "Request.hpp"
# include "User.hpp"
# include "Channel.hpp"

# define BACKLOG 10
# define MAX_CONNECTIONS 1024 
# define BUFFER_SIZE 512
# define CHANNEL_PER_USER_LIMIT 5

const std::string SERVER_NAME = "42_IRC";
class User;
class Request;

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

// noumeric replies to users
namespace RPL {
	const std::string RPL_CHANNELMODEIS = "324";
	const std::string RPL_CREATIONTIME = "329";
	const std::string RPL_NOTOPIC = "331";
	const std::string RPL_TOPIC = "332";
	const std::string RPL_TOPICWHOTIME = "333";	
	const std::string RPL_NAMREPLY = "353";	
	const std::string RPL_INVITING = "341";
}

// errors starting with 9 are custom
namespace Error {
	const std::string ERR_ALREADYREGISTERED = "462";
	const std::string ERR_NEEDMOREPARAMS = "461";
	const std::string ERR_PASSWDMISMATCH = "464";
	const std::string ERR_NICKNAMEINUSE = "433";
	const std::string ERR_NONICKNAMEGIVEN = "431";
	const std::string ERR_NOAUTHENTICATION = "998";
	const std::string ERR_ERRONEUSNICKNAME = "432";
	const std::string ERR_UNKNOWNCOMMAND = "421";
	const std::string ERR_TOOMANYCHANNELS = "405";
	const std::string ERR_NOSUCHCHANNEL = "403";
	const std::string ERR_ILLEGALCHANNELNAME = "479";
	const std::string ERR_BADCHANNELKEY = "475";
	const std::string ERR_CHANNELISFULL = "471";
	const std::string ERR_NOTREGISTERED = "451";
	const std::string ERR_NORECIPIENT = "411";
	const std::string ERR_NOTEXTTOSEND = "412";
	const std::string ERR_NOSUCHNICK = "401";
	const std::string ERR_CANNOTSENDTOCHAN = "404";
	const std::string ERR_TOOMANYTARGETS = "997";
	const std::string ERR_USERSDONTMATCH = "502";
	const std::string ERR_MODENOTFORCHANNEL = "996";
	const std::string ERR_CHANOPRIVSNEEDED = "482";
	const std::string ERR_UNKNOWNMODE = "472";
	const std::string ERR_USERNOTINCHANNEL = "441";
	const std::string ERR_INVALIDMODEPARAM = "696";
	const std::string ERR_INVALIDKEY = "525";
	const std::string ERR_NOTONCHANNEL = "442";
	const std::string ERR_USERONCHANNEL = "443";
	const std::string ERR_INVITEONLYCHAN = "473";
}

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
		std::vector<Channel>		_channels;
		std::vector<User>			_users;		
		
		public:
		Server();
		Server(int port, std::string password);
		
		void		start();
		Request 	parse(std::string input) const;
		void		execute(Request request, int user_index);
		void		listentosocket();
		void		close_and_free_socket(std::string err_msg);
		void		accept_connection();
		void		close_connection(int user_index);
		void		communicate(int i);
		static void	signal_handler(int signal);
		pollfd		init_pollfd();
		void		send_data(int n, std::string str);
		void		print_msg_to_user(std::string msg, int user_index);
		void		print_msg_to_user_with_nickname(std::string msg, std::string nickname);
		void		print_error_to_user(std::string numeric, std::string error_msg, int user_index);
		void		print_reply_to_user(std::string numeric, std::string msg, int user_index);
		void		print_msg_to_channel(std::string msg, std::string channel, std::string sender_nick);
		void 		print_reply_to_channel(std::string numeric, std::string msg, std::string channel);
		bool 		does_user_exist(std::string nickname);
		int 		get_channel_index(std::string channel_name);

		// commands
		void		pass(Request request, int user_index);
		void		nick(Request request, int user_index);
		void		user(Request request, int user_index);
		void		quit(Request request, int user_index);
		void		join(Request request, int user_index);
		void		privmsg(Request request, int user_index, bool is_notice);
		void		mode(Request request, int user_index);
		void		topic(Request request, int user_index);
		void		kick(Request request, int user_index);
		void		invite(Request request, int user_index);
};

#endif