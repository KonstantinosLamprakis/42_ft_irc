#include "../headers/Channel.hpp"

std::string	Channel::choose_type(std::string name) //Channels with +, #, & have a limited lifetime; they are deleted after every user left the channel
{
	char	c = name[0];
	std::string type_name;
	switch (c)
	{
	case '+':
		type_name = "no_channel_modes_allwd";
		break;
	case '&':
		type_name = "local";
		break;
	case '#':
		type_name = "normal";
		break;
	case '!':
		type_name = "safe"; // will stay even if last user left the chat (all other channel types will be deleted)
		break;
	default:
		throw ChannelCreationFailed();
		break;
	}
	return (type_name);
}

void	Channel::add_channel_mode(char c)
{
	for (unsigned long i = 0; i < this->_server->_channel_modes_allowed.size(); i++)
	{
		if (this->_server->_channel_modes_allowed[i] == c)
		{
			for (unsigned long n = 0; n < sizeof(this->_channel_modes); n++)
			{
				if (this->_channel_modes[n] == c)
					return ;
			}
			this->_channel_modes.push_back(c);
		}
	}
}

void	Channel::remove_channel_mode(char c)
{
	for (unsigned long i = 0; i < this->_server->_channel_modes_allowed.size(); i++)
	{
		if (this->_server->_channel_modes_allowed[i] == c)
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
	}
}

Channel::Channel(std::string name, User creator)
{
	if (check_name_valid(name) != 0)
		throw ChannelCreationFailed();
	// this->_channel_modes = {};
	this->_name = name;
	try
	{
		this->_channel_type = choose_type(name);
	}
	catch (std::exception &e)
	{
		throw e;
	}
	this->_users.push_back(creator);
	if (this->_channel_type == "no_channel_modes_allwd")
	{
		creator.add_channel({name, {'-'}}); //if a user creates a new channel, its directly added to its "library" for + without operators
		this->_channel_modes.push_back('t');
	}
	else
	{
		creator.add_channel({name, {'o'}}); //if a user creates a new channel, its directly added to its "library" with Operator mode
		this->_channel_modes.push_back('t'); // here others could be added, for + channel that is not possible
	}
	if (_channel_type != "safe")
	this->_topic = "";
}

int	Channel::check_name_valid(std::string name)
{
	std::string	name_ins;

	name_ins = toUppercase(name); // channel names are case insensitive
	if (name_ins.length() > 50)
		return (1);
	if 	(name_ins.find(' ') != std::string::npos || name_ins.find(':') != std::string::npos || \
		name_ins.find(7) != std::string::npos || name_ins.find(',') != std::string::npos)
		return (1);
	if (name_ins[0] != '&' && name_ins[0] != '#' && name_ins[0] != '+' && name_ins[0] != '!')
		return (1);
	return (0);
}
