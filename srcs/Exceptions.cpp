#include "Exceptions.hpp"

const char* ServerConnectionFailed::what() const throw()
{
	return ("The server connection couldn't be established.");
}
