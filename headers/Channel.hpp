#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <ctime>

# include "Exceptions.hpp"
# include "Utils.hpp"

# define DEFAULT_MAX_USERS_PER_CHANNEL 3 // this can be bypassed with operator's mode +l command
# define MAX_USERS_PER_CHANNEL 100 // this can NOT be bypassed even with operator's mode +l command
# define MAX_TARGETS_ON_PRIVMSG 7

const std::string CHANNEL_TYPE = "#&!+";
const std::string CHANNEL_PREFIX = "~@&%+";

class Channel{
	private:
		std::string				_name;
		std::string				_key;
		std::vector<std::string>	_users;
		std::vector<std::string>	_operators;
		std::vector<std::string>	_invited_users;
		std::string				_topic;
		std::vector<char>		_channel_modes;
		std::time_t 			_creation_timestamp;
		std::time_t 			_topic_modification_timestamp;
		std::string 			_topic_creator;
		unsigned int			_max_users;

	public:
	Channel(std::string name, std::string key, std::string creator);

	
	// setters
	bool		add_channel_mode(char c);
	bool		remove_channel_mode(char c);	
	void 		add_user(std::string user, std::string key);
	void 		add_operator(std::string user);
	bool 		remove_operator(std::string user);
	bool		remove_user(std::string user);
	void		set_topic(std::string topic, std::string creator);
	void 		set_max_users(unsigned int max_users);
	void 		set_key(std::string key);
	void 		clear_topic();
	void 		add_invited_user(std::string user);
	void	 	remove_invited_user(std::string user);
	bool	 	is_user_invited(std::string user);

	// getters
	std::string	get_modes();
	std::string	get_modes_with_values();
	std::vector<std::string> get_users() const;
	bool 		is_user_in_channel(const std::string nickname) const;
	bool 		is_user_operator(const std::string nickname) const;
	std::string	get_topic() const;
	std::string	get_topic_info() const;
	std::string	get_name() const;
	std::string get_creation_timestamp() const;
};

#endif