#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP


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

#endif