#include "../headers/Channel.hpp"

std::string	Channel::choose_type(std::string name)
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
		type_name = "safe";
		break;
	default:
		throw ChannelCreationFailed();
		break;
	}
	return (type_name);
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
	if (_channel_type != "no_channel_modes_allwd")
	{
		creator.add_channel({name, {'-'}}); //if a user creates a new channel, its directly added to its "library"

	}
	else
		creator.add_channel({name, {'O'}}); //if a user creates a new channel, its directly added to its "library"
	if (_channel_type != "safe")
		creator.add_user_mode(name, 'o');
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