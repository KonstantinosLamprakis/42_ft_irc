#include "../headers/Channel.hpp"

Channel::Channel(std::string name, std::string key, std::string creator)
{
	this->_name = name;
	this->_key = key;
	this->_founder = creator;
	this->_users.push_back(creator);
	this->_operators.push_back(creator);
	this->_topic = "";
	this->_topic_modification_timestamp = 0;
	this->_topic_creator = "";
	this->_creation_timestamp = std::time(nullptr);
	this->_max_users = DEFAULT_MAX_USERS_PER_CHANNEL;
}

bool Channel::add_channel_mode(char c)
{
	for (unsigned long i = 0; i < this->_channel_modes.size(); i++)
	{
		if (this->_channel_modes[i] == c)
			return (false);
	}
	this->_channel_modes.push_back(c);
	if (c == 'i')
		this->_invited_users.clear();
	return (true);
}

bool Channel::remove_channel_mode(char c)
{
	for (unsigned long i = 0; i < this->_channel_modes.size(); i++)
	{
		if (this->_channel_modes[i] == c)
		{
			this->_channel_modes.erase(this->_channel_modes.begin() + i--);
			return (true);
		}
	}
	return (false);
}

void Channel::add_user(std::string user, std::string key){
	if (this->_users.size() >= this->_max_users)
		throw MaxNumberOfUsersInChannel();

	if (this->_key != "" && this->_key != key){
		throw IncorrectKeyForChannel();
	}

	for (unsigned long i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i] == user)
			return; 
	}
	this->_users.push_back(user);
}

/**
 * @brief Remove user/operator if exists from channel, do nothing if doesn't exists
 * 
 * @param user 
 * @return true if user was removed, false if user was not found
 */
bool Channel::remove_user(std::string user){
	std::string user_uppercase = to_uppercase(user);
	bool is_user_found = false;
	for (unsigned long i = 0; i < this->_users.size(); i++)
	{
		if (to_uppercase(this->_users[i]) == user_uppercase){
			this->_users.erase(this->_users.begin() + i--);
			is_user_found = true;
			break;
		}
	}
	for (unsigned long i = 0; i < this->_operators.size(); i++)
	{
		if (to_uppercase(this->_operators[i]) == user_uppercase){
			this->_operators.erase(this->_operators.begin() + i--);
			break;
		}
	}
	return (is_user_found);
}

std::vector<std::string> Channel::get_users() const{
	return (this->_users); // returns by value, not a pointer, so caller can not modify the _users inside the class
}

std::string	Channel::get_topic() const{
	return (this->_topic);
}

void Channel::clear_topic(){
	this->_topic = "";
	this->_topic_modification_timestamp = 0;
	this->_topic_creator = "";
}

void Channel::set_topic(std::string topic, std::string creator){
	this->_topic_modification_timestamp = std::time(nullptr);
	this->_topic_creator = creator;
	this->_topic = topic;
}

std::string	Channel::get_topic_info() const{
	if (this->_topic.empty()) return ("");
	return (this->_topic_creator + " " + std::to_string(this->_topic_modification_timestamp));
}

std::string	Channel::get_name() const{
	return (this->_name);
}

bool Channel::is_user_in_channel(const std::string nickname) const {
	const std::string nickname_Uppercase = to_uppercase(nickname);

    for (unsigned long i = 0; i < this->_users.size(); i++) {
		if (to_uppercase(this->_users[i]) == nickname_Uppercase)
			return (true);
	}
	return (false);
}

bool Channel::is_user_operator(const std::string nickname) const {
	const std::string nickname_Uppercase = to_uppercase(nickname);

    for (unsigned long i = 0; i < this->_operators.size(); i++) {
		if (to_uppercase(this->_operators[i]) == nickname_Uppercase)
			return (true);
	}
	return (false);
}

std::string	Channel::get_modes(){
	if (this->_channel_modes.empty())
		return ("");

	std::string result(this->_channel_modes.begin(), this->_channel_modes.end());
	return ("+" + result);
}

std::string	Channel::get_modes_with_values(){
	std::string modes = this->get_modes();
	std::string values = "";
	for (unsigned long i = 0; i < this->_channel_modes.size(); i++)
	{
		if (this->_channel_modes[i] == 'k') values += " " + this->_key;
		if (this->_channel_modes[i] == 'l') values += " " + std::to_string(this->_max_users);
	}
	return (modes + values);
}

std::string Channel::get_creation_timestamp() const {
	return (std::to_string(this->_creation_timestamp));
}

void Channel::set_max_users(unsigned int max_users){
	this->_max_users = max_users;
}

void Channel::add_operator(std::string user){
	const std::string user_uppercase = to_uppercase(user);
	for (unsigned long i = 0; i < this->_operators.size(); i++)
	{
		if (to_uppercase(this->_operators[i]) == user_uppercase){
			return;
		}
	}
	this->_operators.push_back(user);
}

bool Channel::remove_operator(std::string user){
	const std::string user_uppercase = to_uppercase(user);
	bool is_user_found = false;
	for (unsigned long i = 0; i < this->_operators.size(); i++)
	{
		if (to_uppercase(this->_operators[i]) == user_uppercase){
			this->_operators.erase(this->_operators.begin() + i--);
			break;
		}
	}
	return (is_user_found);
}

void Channel::add_invited_user(std::string user){
	const std::string user_uppercase = to_uppercase(user);
	for (unsigned long i = 0; i < this->_invited_users.size(); i++)
	{
		if (to_uppercase(this->_invited_users[i]) == user_uppercase){
			return;
		}
	}
	this->_invited_users.push_back(user);
}

void Channel::remove_invited_user(std::string user){
	const std::string user_uppercase = to_uppercase(user);
	for (unsigned long i = 0; i < this->_invited_users.size(); i++)
	{
		if (to_uppercase(this->_invited_users[i]) == user_uppercase){
			this->_invited_users.erase(this->_invited_users.begin() + i--);
			break;
		}
	}
}

bool Channel::is_user_invited(std::string user){
	const std::string user_uppercase = to_uppercase(user);
	for (unsigned long i = 0; i < this->_invited_users.size(); i++)
	{
		if (to_uppercase(this->_invited_users[i]) == user_uppercase)
			return (true);
	}
	return (false);
}

void Channel::set_key(std::string key){
	this->_key = key;
}

void Channel::rename_user(std::string current_nickname, std::string new_nickname){
	std::string user_uppercase = to_uppercase(current_nickname);
	for (unsigned long i = 0; i < this->_users.size(); i++)
	{
		if (to_uppercase(this->_users[i]) == user_uppercase){
			this->_users[i]	= new_nickname;
			break;
		}
	}
	for (unsigned long i = 0; i < this->_operators.size(); i++)
	{
		if (to_uppercase(this->_operators[i]) == user_uppercase){
			this->_operators[i] = new_nickname;
			break;
		}
	}
	for (unsigned long i = 0; i < this->_invited_users.size(); i++)
	{
		if (to_uppercase(this->_invited_users[i]) == user_uppercase){
			this->_invited_users[i] = new_nickname;
			break;
		}
	}
}

std::string Channel::get_names() const{
	std::string names = "";
	for (unsigned long i = 0; i < this->_users.size(); i++)
	{
		if (this->_founder == this->_users[i])
			names += "~";
		else if (this->is_user_operator(this->_users[i]))
			names += "@";
		names += this->_users[i];
		if (i != this->_users.size() - 1)
			names += ", ";
	}
	return (names);
}
