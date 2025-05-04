#include "../headers/Channel.hpp"

Channel::Channel(std::string name, std::string key, std::string creator)
{
	this->_name = name;
	this->_key = key;
	this->_users.push_back(creator);
	this->_operators.push_back(creator);
	this->_topic = "";
	this->_creation_timestamp = std::time(nullptr);
}

void Channel::add_channel_mode(char c)
{
	for (unsigned long n = 0; n < sizeof(this->_channel_modes); n++)
	{
		if (this->_channel_modes[n] == c)
			return ;
	}
	this->_channel_modes.push_back(c);
}

void Channel::remove_channel_mode(char c)
{
	for (unsigned long n = 0; n < sizeof(this->_channel_modes); n++)
	{
		if (this->_channel_modes[n] == c)
		{
			this->_channel_modes.erase(this->_channel_modes.begin() + n);
			return ;
		}
	}
}

void Channel::add_user(std::string user, std::string key){
	if (this->_users.size() >= MAX_USERS_PER_CHANNEL)
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
	bool is_user_found = false;
	for (unsigned long i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i] == user){
			this->_users.erase(this->_users.begin() + i);
			is_user_found = true;
			break;
		}
	}
	for (unsigned long i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i] == user){
			this->_operators.erase(this->_operators.begin() + i);
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

void Channel::set_topic(std::string topic){
	this->_topic = topic;
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

std::string	Channel::get_modes(){
	if (this->_channel_modes.empty())
		return ("");

	std::string result(this->_channel_modes.begin(), this->_channel_modes.end());
	return (result);
}

std::string Channel::get_creation_timestamp() const {
	return (std::to_string(this->_creation_timestamp));
}