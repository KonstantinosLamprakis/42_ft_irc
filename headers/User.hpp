#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <vector>
# include <map>
# include "Channel.hpp"
# include "Server.hpp"

class User{
	private:
		std::string 								_username;
		std::string									_fullname;
		std::string									_nickname;

		int											_fd;
		int											_status_usr_creation;
		std::vector<std::string>					_channels;
		bool										_is_authenticated; // though PASS command
		bool										_is_registered; // through PASS, NICK and USER commands
		bool										_is_nickname_set;
		bool										_is_username_set;
		class Server*								_server;

		public:
		User(Server* s);
		User(int fd, Server* s);

		// getters
		bool		is_authenticated() const;
		bool		is_registered() const;
		std::string	get_username() const;
		std::string	get_fullname() const;
		std::string	get_nickname() const;
		bool		is_user_in_channel(std::string channel_name) const;

		// setters
		void	set_username(std::string nickname);
		void	set_fullname(std::string nickname);
		void	set_nickname(std::string nickname);
		void	set_authenticated(bool authenticated);
		void	add_channel(std::string new_channel);
		void	remove_channel(std::string channel);
};

#endif