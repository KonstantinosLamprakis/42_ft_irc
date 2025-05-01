#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <vector>
# include "Channel.hpp"

class User{
	private:
		std::string 			_username;
		std::string				_fullname;
		std::string				_nickname;

		int						_fd;
		int						_status_usr_creation;
		std::vector<char>		_avlb_user_modes; // seems like we only need the one for operator mode
		std::vector<char>		_user_modes;
		// std::vector<Channel>	_channels;
		bool					_is_authenticated; // though PASS command
		bool					_is_registered; // through PASS, NICK and USER commands

	public:
		User();
		User(int fd);
		void	add_mode(char c);
		void	remove_mode(char c);

		// getters
		bool is_authenticated() const;
		bool is_registered() const;
		std::string get_username() const;
		std::string get_fullname() const;
		std::string get_nickname() const;

		// setters
		void set_username(std::string nickname);
		void set_fullname(std::string nickname);
		void set_nickname(std::string nickname);
		void set_authenticated(bool authenticated);
		void set_registered(bool registered);
};

#endif

//all available User modes - reduce in Server Contructor if not needed
// a - user is flagged as away;
// i - marks a users as invisible;
// w - user receives wallops;
// r - restricted user connection;
// o - operator flag;
// O - local operator flag;
// s - marks a user for receipt of server notices.