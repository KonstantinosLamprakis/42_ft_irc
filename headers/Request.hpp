#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
# include "Channel.hpp"
# include "User.hpp"

class Request{
	private:
		std::string					_cmd;
		// Channel						_cnl;
		// User						_usr; // request can be either sent to a channel OR a user (i think)
		std::vector<std::string>	_args;
	public:
		Request(std::string const cmd, std::vector<std::string> const args);
		Request(Request const &copy);
		Request &operator=(Request const &old);
		~Request();

		std::string const getCommand() const;
		std::vector<std::string> const getArgs() const;
};

#endif