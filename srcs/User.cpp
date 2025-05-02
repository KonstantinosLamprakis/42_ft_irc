#include "../headers/User.hpp"

User::User(Server* s)
{
	this->_username = "";
	this->_fullname = "";
	this->_nickname = "*";

	this->_fd = -1;
	this->_status_usr_creation = 0; // e.g. 0 for connection and the three remaining fields need to be filled, which increases value + 1; if already set (not "" anymore) - no increase
	this->_is_authenticated = false; 
	this->_is_registered = false;
	this->_server = s;
}

User::User(int fd, Server* s)
{
	this->_username = "";
	this->_fullname = "";
	this->_nickname = "*";

	this->_fd = fd;
	this->_status_usr_creation = 0; // e.g. 0 for connection and the three remaining fields need to be filled, which increases value + 1; if already set (not "" anymore) - no increase
	this->_is_authenticated = false; 
	this->_is_registered = false;
	this->_server = s;
}

void	User::add_user_mode(std::string channel_name, char c)
{
	std::map<std::string, std::vector<char>>::iterator	it;

	for (unsigned long i = 0; i < this->_server->_avlb_user_modes.size(); i++)
	{
		if (this->_server->_avlb_user_modes[i] == c)
		{
			it = this->_channels.find(channel_name);
			if (it == this->_channels.end())
				return ; // if necessary print statement
			for (unsigned long n = 0; n < sizeof(this->_channels.find(channel_name)->second); n++)
			{
				if (this->_channels.find(channel_name)->second[n] == c)
					return ; // if necessary print
			}
			this->_channels.find(channel_name)->second.push_back(c);
		}
	}
}

void	User::remove_user_mode(std::string channel_name, char c)
{
	std::map<std::string, std::vector<char>>::iterator	it;

	for (unsigned long i = 0; i < this->_server->_avlb_user_modes.size(); i++)
	{
		if (this->_server->_avlb_user_modes[i] == c)
		{
			it = this->_channels.find(channel_name);
			if (it == this->_channels.end())
				return ; // if necessary print statement
			for (unsigned long n = 0; n < sizeof(this->_channels.find(channel_name)->second); n++)
			{
				if (this->_channels.find(channel_name)->second[n] == c)
					this->_channels.find(channel_name)->second.erase(this->_channels.find(channel_name)->second.begin() + n);
				}
			return ; // if necessary print
		}
	}
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

void	User::set_registered(bool registered)
{
	this->_is_registered = registered;
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

int User::search_channel(std::string name) const
{
	if (this->_channels.find(name) != _channels.end())
		return (0); // returns true or false depending on creator mode or not
	return (1); //return -1 if user is not in the channel
}


void User::set_nickname(std::string nickname){
	this->_nickname = nickname;
}

void User::set_username(std::string username){
	this->_username = username;
}

void User::set_fullname(std::string fullname){
	this->_fullname = fullname;
}

void User::add_channel(std::pair<std::string, std::vector<char>> new_channel)
{
	this->_channels.insert(new_channel);
}

void User::remove_channel(std::string channel)
{
	this->_channels.erase(channel);
}