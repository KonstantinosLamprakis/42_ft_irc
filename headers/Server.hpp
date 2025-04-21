#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>

# include "Request.hpp"

const std::string SPACE = "\t\n ";
const std::string CRLF = "\r\n";


namespace Command {
    const std::string PASS = "PASS";
    const std::string NICK = "NICK";
    const std::string USER = "USER";
    const std::string QUIT = "QUIT";
};

class Server{
	private:
		// variables
		int	_port;
		std::string _password;

		// functions
		Request _parse(std::string input) const;
		void _execute(Request request);

	public:
		Server();
		Server(int port, std::string password);
		Server(Server &copy);
		Server &operator=(Server &old);
		~Server();
		void start();
};

#endif