#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>

class Channel{
	private:
		std::string	_name;
		std::vector<std::string> _users;

	public:
	Channel();
	Channel(Channel &copy);
	Channel& operator=(Channel &old);
	~Channel();
};

#endif