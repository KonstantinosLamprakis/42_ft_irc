#include "../headers/Exceptions.hpp"

const char* ServerConnectionFailed::what() const throw()
{
	return ("The server connection couldn't be established.");
}

const char* ClientConnectionFailed::what() const throw()
{
	return ("The client connection couldn't be established.");
}

const char* ChannelCreationFailed::what() const throw()
{
	return ("The name entered is not a valid Channel name.");
}

const char* MaxNumberOfUsersInChannel::what() const throw()
{
	return ("The max amount of users is reached. No other user can be added.");
}

const char* IncorrectKeyForChannel::what() const throw()
{
	return ("The key that was provided is wrong.");
}

