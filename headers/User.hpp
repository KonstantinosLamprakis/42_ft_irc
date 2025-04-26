#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <vector>
# include "Channel.hpp"

class User{
	private:
		std::string _username;
		std::string	_fullname;
		std::string	_nickname;

		int	_fd;
		//char[]					_user_modes;
		//std::vector<Channel>		_channels;

	public:
		User();
		User(int fd);
		User(User &copy);
		User& operator=(User &old);
		~User();
};

#endif