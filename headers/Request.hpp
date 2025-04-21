#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <vector>


class Request{
	private:
		std::string					_cmd;
		std::vector<std::string>	_args;
	public:
		Request(std::string cmd, std::vector<std::string> args);
		Request(Request &copy);
		Request& operator=(Request &old);
		~Request();

		std::string const getCommand() const;
};

#endif