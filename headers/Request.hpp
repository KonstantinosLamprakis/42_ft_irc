#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
# include "Channel.hpp"
# include "User.hpp"

class Request{
	private:
		std::string					_cmd;
		// Channel					_cnl; // maybe included in _args - has to be clear for server than or handling of sending in commans
		// User						_usr; // request can be either sent to a channel OR a user (i think) - see comment above (but i think it makes sense to use one function for all sends so maybe we double store it to always use the same command) or just use the arg as input for send function instead of request(but we need receipients as well)
		std::vector<std::string>	_args; // incl. message that has to be sent (if necessary)

		public:
		Request(std::string const cmd, std::vector<std::string> const args);
		Request(Request const &copy);
		Request &operator=(Request const &old);
		~Request();

		std::string const getCommand() const;
		std::vector<std::string> const getArgs() const;
};

#endif