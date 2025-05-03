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

class	UserAdditionFailed : public std::exception
{
	public:
		const char* what() const throw();
};

#endif