#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include "Exceptions.hpp"

# define MAX_USERS_PER_CHANNEL 3

class Channel{
	private:
		std::string				_name;
		std::vector<std::string>	_users;
		std::vector<std::string>	_operators;
		std::string				_topic;
		std::vector<char>		_channel_modes;

	public:
	Channel(std::string name, std::string creator);

	void		add_channel_mode(char c);
	void		remove_channel_mode(char c);

	void 		add_user(std::string user);
	void		remove_user(std::string user);

	std::string	get_topic() const;
	void		set_topic(std::string topic);

	std::string	get_name() const;
};

#endif