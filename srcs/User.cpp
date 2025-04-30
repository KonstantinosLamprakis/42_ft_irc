#include "../headers/User.hpp"

User::User()
{
	this->_username = "";
	this->_fullname = "";
	this->_nickname = "";

	this->_fd = -1;
	this->_status_usr_creation = 0; // e.g. 0 for connection and the three remaining fields need to be filled, which increases value + 1; if already set (not "" anymore) - no increase
	this->_avlb_user_modes = {'a', 'i', 'w', 'r', 'o', 'O', 's'}; // seems like we only need the one for operator mode
	this->_is_authenticated = false; 
	this->_is_registered = false;
}

User::User(int fd)
{
	this->_username = "";
	this->_fullname = "";
	this->_nickname = "";

	this->_fd = fd;
	this->_status_usr_creation = 0; // e.g. 0 for connection and the three remaining fields need to be filled, which increases value + 1; if already set (not "" anymore) - no increase
	this->_avlb_user_modes = {'a', 'i', 'w', 'r', 'o', 'O', 's'}; // seems like we only need the one for operator mode
	this->_is_authenticated = false; 
	this->_is_registered = false;
}

void	User::add_mode(char c) // can be used in MODE command
{
	for (unsigned long i = 0; i < sizeof(this->_avlb_user_modes); i++)
	{
		if (this->_avlb_user_modes[i] == c)
		{
			for (unsigned long n = 0; n < sizeof(this->_user_modes); n++)
			{
				if (this->_user_modes[n] == c)
					return ; // print message if necessary
				
			}
			this->_user_modes.push_back(c);
		}
	}
}

void	User::remove_mode(char c) // can be used in MODE command
{
	for (unsigned long i = 0; i < sizeof(this->_avlb_user_modes); i++)
	{
		if (this->_avlb_user_modes[i] == c)
		{
			for (unsigned long n = 0; n < sizeof(this->_user_modes); n++)
			{
				if (this->_user_modes[n] == c)
					this->_user_modes.erase(this->_user_modes.begin() + i); //print message if necessary
			}
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
