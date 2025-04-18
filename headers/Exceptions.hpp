#pragma once
#include "Server.hpp"


class	ServerConnectionFailed : public std::exception
{
	public:
		const char* what() const throw();
};
