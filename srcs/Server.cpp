#include "../headers/Server.hpp"

bool Server::_signal_status = false;

pollfd	Server::init_pollfd()
{
	struct pollfd	new_pollfd;

	memset(&new_pollfd, 0, sizeof(new_pollfd));
	new_pollfd.fd = -1;
	new_pollfd.events = POLLIN | POLLHUP;
	new_pollfd.revents = 0;
	return (new_pollfd);
}

Server::Server(){
	this->_port = -1;
	this->_password = "";
	this->_connection_fds.push_back(init_pollfd());
	this->_users.push_back(User(this->_sockfd, this)); // Adding server as placeholder to keep _users and _connection_fds indexes in sync
	this->_sockfd = -1;
	this->_server_info = NULL;
	this->_amnt_connections = 0;
}

Server::Server(int port, std::string password)
{
	this->_port = port;
	this->_password = password;
	this->_connection_fds.push_back(init_pollfd());
	this->_users.push_back(User(this->_sockfd, this)); // Adding server as placeholder to keep _users and _connection_fds indexes in sync
	this->_sockfd = -1;
	this->_server_info = NULL;
	this->_amnt_connections = 0;
}

/**
 * @brief sends data to all other connected users
 * @param n iterator refering to pollfd 
 * @param str message to be send
 * 
 */
void	Server::send_data(int n, std::string str) //  can be done if request exists (sening msg to either channel or user or all)
{
	int	message_length = sizeof(str);
	for (int i = 1; i < this->_amnt_connections; i++) // for everyone but host ...
	{
		int data_sent = 0;
		while ((data_sent < message_length) && (n != i)) // .. and sender 
		{
			data_sent += send(this->_connection_fds[i].fd, &str[data_sent], (message_length - data_sent), 0);
			if (data_sent < 0)
			{
				std::cout << "sending message to connection fd " << this->_connection_fds[i].fd << " failed" << std::endl;
				continue ;
			}
		}
	}
}

/**
 * @brief prints the message msg to the spesific user
 * 
 * @param msg 
 * @param user_index 
 */
void	Server::print_msg_to_user(std::string msg, int user_index){
	if (send(this->_connection_fds[user_index].fd, msg.c_str(), msg.length(), 0) == -1)
		std::cout << "send() error for fd: " << this->_connection_fds[user_index].fd << ": " << strerror(errno) << std::endl;
}

/**
 * @brief prints a message to a user with a spesific nickname who should be registered
 * 
 * @param msg 
 * @param nickname 
 */
void Server::print_msg_to_user_with_nickname(std::string msg, std::string nickname){
	std::string uppercase_nickname = to_uppercase(nickname);
	for (unsigned long i = 0; i < this->_users.size(); i++)
	{
		if (to_uppercase(this->_users[i].get_nickname()) == uppercase_nickname){
			if (!this->_users[i].is_registered()) continue;
			print_msg_to_user(msg, i);
			return;
		}
	}
	throw UserNotFound();
}

/**
 * @brief Prints message to all users of a channel except the sender
 * 
 * @param msg 
 * @param channel 
 * @param sender_nick 
 */
void Server::print_msg_to_channel(std::string msg, std::string channel, std::string sender_nick){
	int channel_index = this->get_channel_index(channel);
	if (channel_index == -1)
		throw ChannelNotFound();
	std::string uppercase_sender_nick = to_uppercase(sender_nick);
	if (!this->_channels[channel_index].is_user_in_channel(sender_nick))
		throw UserNotInChannel();
	std::vector<std::string> channel_users = this->_channels[channel_index].get_users();
	for (unsigned long j = 0; j < channel_users.size(); j++) {
		if (to_uppercase(channel_users[j]) == uppercase_sender_nick) continue; // skip the user who sent the message
		print_msg_to_user_with_nickname(msg, channel_users[j]);
	}
	return;
}

/**
 * @brief prints error values to a spesific user
 * 
 * @param numeric numeric should be of type namespace Error in server
 * @param error_msg 
 * @param user_index 
 */
void Server::print_error_to_user(std::string numeric, std::string error_msg, int user_index){
	std::string target = this->_users[user_index].get_nickname();
	std::string final_msg = ":" + SERVER_NAME + " " + numeric + " " + target + " " + error_msg + "\n";
	if (send(this->_connection_fds[user_index].fd, final_msg.c_str(), final_msg.length(), 0) == -1)
		std::cout << "send() error for fd: " << this->_connection_fds[user_index].fd << ": " << strerror(errno) << std::endl;
}

/**
 * @brief prints noumeric RPL (reply) to a spesific user
 * 
 * 
 * @param numeric numeric should be of type namespace Error in server
 * @param msg 
 * @param user_index 
 */
void Server::print_reply_to_user(std::string numeric, std::string msg, int user_index){
	std::string target = this->_users[user_index].get_nickname();
	std::string final_msg = ":" + SERVER_NAME + " " + numeric + " " + target + " " + msg + "\n";
	if (send(this->_connection_fds[user_index].fd, final_msg.c_str(), final_msg.length(), 0) == -1)
		std::cout << "send() error for fd: " << this->_connection_fds[user_index].fd << ": " << strerror(errno) << std::endl;
}

void Server::print_reply_to_channel(std::string numeric, std::string msg, std::string channel){
	int channel_index = this->get_channel_index(channel);
	if (channel_index == -1)
		throw ChannelNotFound();
	std::vector<std::string> channel_users = this->_channels[channel_index].get_users();
	for (unsigned long j = 0; j < channel_users.size(); j++){
		std::string final_msg = ":" + SERVER_NAME + " " + numeric + " " + channel_users[j] + " " + msg + "\n";
		print_msg_to_user_with_nickname(final_msg, channel_users[j]);
	}
}

void Server::close_connection(int user_index){
	close (this->_connection_fds[user_index].fd);
	this->_connection_fds.erase(_connection_fds.begin() + user_index);
	this->_users.erase(_users.begin() + user_index);
	for (unsigned long i = 0; i < this->_channels.size(); i++){
		this->_channels[i].remove_user(this->_users[user_index].get_nickname());
		// TODO(KL) should I print a message to the channel that the user left?
		if (this->_channels[i].get_users().empty()){
			this->_channels.erase(_channels.begin() + i--);
		}
	}
	this->_amnt_connections--;
}
/**
 * @brief read user input, call the parser and execute the command
 * 
 * Edge cases:
 * - empty input: ignore
 * - input more than the limit (512) -> error
 * - input a command that doesn't exist -> error
 * 
 * @param i iterator refering to pollfd
 */
void	Server::communicate(int i)
{
	char		buff[BUFFER_SIZE + 1]; //buffer to receive data
	int			bytes_recvd = BUFFER_SIZE; //to store return value of recv
	std::string	str = "";

	memset(&buff, 0, sizeof(buff));
	while (bytes_recvd == BUFFER_SIZE)
	{
		bytes_recvd = recv(this->_connection_fds[i].fd, buff, BUFFER_SIZE, 0);
		if (bytes_recvd < 0 || (bytes_recvd == 0 && str == ""))
		{
			if (bytes_recvd < 0)
				std::cout << "recv at fd " << this->_connection_fds[i].fd << " failed" << std::endl;
			else
				std::cout << "client " << this->_connection_fds[i].fd << " closed the connection" << std::endl;
			close_connection(i);
			return ;
		}
		buff[bytes_recvd + 1] = '\0';
		str = str + buff;
	}
	if (str.empty() || str == "\n") return ; // IRC Server must ignore empty lines
	if (str.length() > 512) this->print_msg_to_user("Error: Input too long.\n", i); // max input length is 512 for IRC
	str.pop_back(); //remove \n at the end
	Request in = parse(str);
	execute(in, i); // probably in next poll - seems like we need to execute in the next while loop (eval sheet)
}

void	Server::accept_connection()
{
	int			new_connection_fd = 0; 
	sockaddr	comm_socket; //input for accept; saves the communication socket for the connection requested
	socklen_t	addrlen;

	addrlen = sizeof(comm_socket);
	new_connection_fd = accept(this->_sockfd, &comm_socket, &addrlen);
	if (new_connection_fd < 0)
		std::cout << "Accept failed" << std::endl;
	else
	{
		this->_connection_fds.push_back(init_pollfd());
		this->_connection_fds[this->_amnt_connections].fd = new_connection_fd;
		this->_amnt_connections++;
		this->_users.push_back(User(new_connection_fd, this));
	}
}

void	Server::listentosocket() //listens to the open socket of the server for incoming clients and requests
{
	int		err = 0; //return value used for error handling
	char	test[1024]; //std::string possible?

	if (listen(this->_sockfd, BACKLOG) != 0)
	{
		std::cout << "Listen failed" << std::endl;
		throw ClientConnectionFailed() ;
	}
	this->_connection_fds[0].fd = this->_sockfd;
	this->_connection_fds[0].events = 0;
	this->_connection_fds[0].events = POLLIN;
	this->_amnt_connections = 1;
	while (this->_signal_status == false)
	{
		err = poll(&this->_connection_fds[0], this->_amnt_connections, -1);
		if (err <= -1 && this->_signal_status == false)
			throw ClientConnectionFailed();
		for (int i = 0; i < this->_amnt_connections; i++)
		{
			if ((this->_connection_fds[i].revents & (POLLIN | POLLHUP)) && this->_signal_status == false) // is anywhere input waiting
			{
				if (this->_connection_fds[i].fd == this->_sockfd)
					this->accept_connection();
				else
					this->communicate(i);
				continue ; // it looks like we need to call poll before EVERY time we call accept, send, recv(eval sheet)
			}
			if ((this->_connection_fds[i].revents & POLLHUP) && this->_signal_status == false)
			{
				if (this->_connection_fds[i].fd == this->_sockfd) // does this make sense? - if bind socket is gone server is off, right?
					break ;
				else
				{
					if (recv(this->_connection_fds[i].fd, test, sizeof(test), 0) == 0)
					{
						close (this->_connection_fds[i].fd);
						this->_connection_fds.erase(_connection_fds.begin() + i--);
					}
				}
			}
		}
	}
}

void	Server::start()
{
	struct	addrinfo	server_hints; //input for getaddrinfo function - contains flags used to define server_inf
	struct	addrinfo*	tmp; //struct to check all generated versions in linked list which one is working
	int					test = 0; //all solutions in the linked list get tested - if one of them works we continue to try bind
	int					yes = 1; //value to set flag in setsockopt function (refered to as address) - probably not necessary and &1 would be enough

	memset(&server_hints, 0, sizeof(server_hints));
	server_hints.ai_family = AF_INET; //only IPv4 (later on we're only allowed to use functions for IPv4)
	server_hints.ai_socktype = SOCK_STREAM; //TCP makes sense for us (option would be UDP)
	server_hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, std::to_string(this->_port).c_str(), &server_hints, &this->_server_info) != 0)
	{
		std::cout << "getaddrinfo failed" << std::endl;
		throw ServerConnectionFailed();
	}
	for (tmp = this->_server_info; tmp != NULL; tmp = tmp->ai_next)
	{
		test = socket(this->_server_info->ai_family, this->_server_info->ai_socktype, this->_server_info->ai_protocol);
		if (test < 0)
			continue ;
		else
		{
			if (setsockopt(test, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) //option to allow reusage of the same port without waiting incl keeping up connections
			{
				close_and_free_socket("setting up socket didn't work properly");
				throw ServerConnectionFailed();
			}
			if (bind(test, this->_server_info->ai_addr, this->_server_info->ai_addrlen) != 0)
			{
				close_and_free_socket("binding socket didn't work");
				throw ServerConnectionFailed();
			}
			this->_sockfd = test;
			if (fcntl(this->_sockfd, F_SETFL, O_NONBLOCK) == -1)
			{
				close_and_free_socket("opening a non-blocking connection didn't work");
				throw ServerConnectionFailed();
			}
			break ;
		}
		if (tmp == NULL)
			throw ServerConnectionFailed();
		close_and_free_socket(std::string());
		throw ServerConnectionFailed();
	}
	try
	{
		this->listentosocket();
	}
	catch(const std::exception &e)
	{
		close_and_free_socket(std::string());
		throw e ;
	}
}

Request Server::parse(std::string input) const {
	std::string command;
	std::vector<std::string> args;
	std::istringstream stream(input);
    std::string word;

    if (stream >> command) {
        while (stream >> word) {
            if (word[0] == ':') {
				std::string remaining_stream;
				std::getline(stream, remaining_stream);
				std::string remaining_word = "";
				if (word.size() > 1) remaining_word = word.substr(1);
				args.push_back(remaining_word + remaining_stream);
				break;
            }
            args.push_back(word);
        }
    }

	return Request(command, args);
}

void Server::execute(Request request, int user_index){
	std::string uppercase_command = to_uppercase(request.getCommand());
	if (uppercase_command == Command::PASS) 
		this->pass(request, user_index);
	else if (uppercase_command == Command::NICK)
		this->nick(request, user_index);
	else if (uppercase_command == Command::USER)
		this->user(request, user_index);
	else if (uppercase_command == Command::JOIN)
		this->join(request, user_index);
	else if (uppercase_command == Command::PRIVMSG)
		this->privmsg(request, user_index, false);
	else if (uppercase_command == Command::NOTICE)
		this->privmsg(request, user_index, true);
	else if (uppercase_command == Command::QUIT)
		this->quit(request, user_index);
	// operator's commands
	else if (uppercase_command == Command::KICK)
		this->kick(request, user_index);
	else if (uppercase_command == Command::INVITE)
		this->invite(request, user_index);
	else if (uppercase_command == Command::TOPIC)
		this->topic(request, user_index);
	else if (uppercase_command == Command::MODE)
		this->mode(request, user_index);
	else
		this->print_error_to_user(Error::ERR_UNKNOWNCOMMAND, request.getCommand() + " :Unknown command.\n", user_index);
}

void	Server::signal_handler(int signal)
{
	(void)signal;
	_signal_status = true;
}

void	Server::close_and_free_socket(std::string err_msg)
{
	int i = 0;

	while ((i < this->_amnt_connections) && (this->_connection_fds[i].fd != -1) && (this->_connection_fds[i].fd != this->_sockfd))
		close (this->_connection_fds[i++].fd);
	if (!err_msg.empty())
		std::cout << err_msg << std::endl;
	freeaddrinfo(this->_server_info);
	close(this->_sockfd);
}

bool Server::does_user_exist(std::string nickname){
	std::string uppercase_nickname = to_uppercase(nickname);
	for (unsigned long i = 0; i < this->_users.size(); i++){
		if (to_uppercase(this->_users[i].get_nickname()) == uppercase_nickname)
			return true;
	}
	return false;
}

/**
 * @brief 
 * 
 * @param channel_name 
 * @return int -1 if not found, index of the channel if found 
 */
int Server::get_channel_index(std::string channel_name){
	std::string uppercase_channel = to_uppercase(channel_name);
	for (unsigned long i = 0; i < this->_channels.size(); i++)
	{
		if (to_uppercase(this->_channels[i].get_name()) == uppercase_channel){
			return (i);
		}
	}
	return (-1);
}
