#ifndef USER_HPP
# define USER_HPP

#include <string>

class User{
	private:
		std::string _username;
		std::string	_fullname;
		std::string	_nickname;

		int	_fd;

	public:
		User();
		User(int fd);
		User(User &copy);
		User& operator=(User &old);
		~User();
};

#endif