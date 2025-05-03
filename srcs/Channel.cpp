#include "../headers/Channel.hpp"

Channel::Channel(std::string name, std::string creator)
{
	this->_name = name;
	this->_users.push_back(creator);
	this->_operators.push_back(creator);
	this->_topic = "";
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

void Channel::add_user(std::string user){
	if (this->_users.size() >= MAX_USERS_PER_CHANNEL)
		throw UserAdditionFailed();

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
 */
void Channel::remove_user(std::string user){
	for (unsigned long i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i] == user){
			this->_users.erase(this->_users.begin() + i);
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