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