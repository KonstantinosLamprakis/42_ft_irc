#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include <exception>

class	ServerConnectionFailed : public std::exception
{
	public:
		const char* what() const throw();
};

class	ClientConnectionFailed : public std::exception
{
	public:
		const char* what() const throw();
};

class	ChannelCreationFailed : public std::exception
{
	public:
		const char* what() const throw();
};

class	MaxNumberOfUsersInChannel : public std::exception
{
	public:
		const char* what() const throw();
};

class	IncorrectKeyForChannel : public std::exception
{
	public:
		const char* what() const throw();
};

class	ChannelNotFound : public std::exception
{
	public:
		const char* what() const throw();
};

class	UserNotInChannel : public std::exception
{
	public:
		const char* what() const throw();
};

class	UserNotFound : public std::exception
{
	public:
		const char* what() const throw();
};

#endif