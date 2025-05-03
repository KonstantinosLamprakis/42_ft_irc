#include "../headers/User.hpp"

User::User(Server* s)
{
	this->_username = ""; // User name anonymous is forbidden
	this->_fullname = "";
	this->_nickname = "*";

	this->_is_nickname_set = false;
	this->_is_username_set = false;
	this->_fd = -1;
	this->_status_usr_creation = 0; // e.g. 0 for connection and the three remaining fields need to be filled, which increases value + 1; if already set (not "" anymore) - no increase
	this->_is_authenticated = false; 
	this->_is_registered = false;
	this->_server = s;
}

User::User(int fd, Server* s)
{
	this->_username = ""; // User name anonymous is forbidden
	this->_fullname = "";
	this->_nickname = "*";

	this->_is_nickname_set = false;
	this->_is_username_set = false;
	this->_fd = fd;
	this->_status_usr_creation = 0; // e.g. 0 for connection and the three remaining fields need to be filled, which increases value + 1; if already set (not "" anymore) - no increase
	this->_is_authenticated = false; 
	this->_is_registered = false;
	this->_server = s;
}

bool	User::is_authenticated() const
{
	return (this->_is_authenticated);
}

bool	User::is_registered() const
{
	return (this->_is_registered);
}

void	User::set_authenticated(bool authenticated)
{
	this->_is_authenticated = authenticated;
}

std::string User::get_nickname() const{
	return this->_nickname;
}

std::string User::get_username() const{
	return this->_username;
}

std::string User::get_fullname() const{
	return this->_fullname;
}

bool User::is_user_in_channel(const std::string channel_name) const {
	const std::string channel_Uppercase = toUppercase(channel_name);

    for (unsigned long i = 0; i < this->_channels.size(); i++) {
		if (toUppercase(this->_channels[i]) == channel_Uppercase)
			return (true);
	}
	return (false);
}

void User::set_nickname(std::string nickname){
	this->_nickname = nickname;
	this->_is_nickname_set = true;
	if (this->_is_username_set && this->_is_nickname_set)
		this->_is_registered = true;
}

void User::set_username(std::string username){
	this->_username = username;
	this->_is_username_set = true;
	if (this->_is_username_set && this->_is_nickname_set)
		this->_is_registered = true;
}

void User::set_fullname(std::string fullname){
	this->_fullname = fullname;
}

void User::add_channel(std::string new_channel)
{
	this->_channels.push_back(new_channel);
}

void User::remove_channel(std::string channel)
{
	for (unsigned long i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i] == channel){
			this->_channels.erase(this->_channels.begin() + i);
			break;
		}
	}
}

int User::get_channel_number() const{
	return this->_channels.size();
}