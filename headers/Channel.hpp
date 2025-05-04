#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <ctime>

# include "Exceptions.hpp"
# include "Utils.hpp"

# define MAX_USERS_PER_CHANNEL 3
# define MAX_TARGETS_ON_PRIVMSG 7

const std::string CHANNEL_MODE = "#&!+";
const std::string CHANNEL_PREFIX = "~@&%+";

class Channel{
	private:
		std::string				_name;
		std::string				_key;
		std::vector<std::string>	_users;
		std::vector<std::string>	_operators;
		std::string				_topic;
		std::vector<char>		_channel_modes;
		std::time_t 			_creation_timestamp;

	public:
	Channel(std::string name, std::string key, std::string creator);

	void		add_channel_mode(char c);
	void		remove_channel_mode(char c);
	std::string	get_modes();

	void 		add_user(std::string user, std::string key);
	bool		remove_user(std::string user);
	std::vector<std::string> get_users() const;
	bool 		is_user_in_channel(const std::string nickname) const;

	std::string	get_topic() const;
	void		set_topic(std::string topic);

	std::string	get_name() const;
	std::string get_creation_timestamp() const;
};

#endif