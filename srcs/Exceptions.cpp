#include "../headers/Exceptions.hpp"

const char* ServerConnectionFailed::what() const throw()
{
	return ("The server connection couldn't be established.");
}

const char* ClientConnectionFailed::what() const throw()
{
	return ("The client connection couldn't be established.");
}