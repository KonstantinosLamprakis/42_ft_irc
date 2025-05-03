#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include "User.hpp"
# include "Exceptions.hpp"
# include "Utils.hpp"
# include "Server.hpp"

class	Server;

class Channel{
	private:
		std::string				_name;
		std::string				_channel_type;
		std::vector<class User>	_users;
		std::string				_topic;
		std::vector<char>		_channel_modes;
		class Server*			_server;

	public:
	Channel(std::string name, User creator);

	int			check_name_valid(std::string name);
	std::string	choose_type(std::string name);
	void		add_channel_mode(char c);
	void		remove_channel_mode(char c);
};

#endif