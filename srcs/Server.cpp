#include "../headers/Server.hpp"
#include "../headers/Exceptions.hpp"
#include "../headers/Helper.hpp"

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

Server::Server() 
{
	this->_port = -1;
	this->_password = "";
	this->_connection_fds.push_back(init_pollfd());
	this->_sockfd = -1;
	this->_server_info = NULL;
	this->_amnt_connections = 0;
}

Server::Server(int port, std::string password){
	this->_port = port;
	this->_password = password;
	this->_connection_fds.push_back(init_pollfd());
	this->_sockfd = -1;
	this->_server_info = NULL;
	this->_amnt_connections = 0;
}

// void	Server::send_data(Request in) //  can be done if request exists (sening msg to either channel or user or all)
// {
// 	(void)in;
// }

/**
 * @brief receives, reads and if necessary forwards message or executes command
 * @param i iterator refering to pollfd
 */

void	Server::communicate(int i)
{
	char		buff[BUFFER_SIZE]; //buffer to receive data
	int			bytes_recvd = BUFFER_SIZE; //to store return value of recv
	std::string	str = "";

	while (bytes_recvd == BUFFER_SIZE)
	{
		bytes_recvd = recv(this->_connection_fds[i].fd, buff, sizeof(buff), 0);
		if (bytes_recvd <= 0)
		{
			if (bytes_recvd < 0)
				std::cout << "recv at fd " << this->_connection_fds[i].fd << " failed" << std::endl;
			else
				std::cout << "client " << this->_connection_fds[i].fd << " closed the connection" << std::endl;
			close (this->_connection_fds[i].fd);
			this->_connection_fds.push_back(init_pollfd());
			this->_amnt_connections--;
			return ;
		}
		str = str + buff;
	}
	try{
		Request in = parse(str);
		execute(in);
	}catch(const std::exception &e){
		std::cout << "Error: " << e.what() << std::endl;
	}
}

void	Server::accept_connection() //accept connections to socket
{
	int				tmp = 0; //temporary storage of the fd for the new connection
	sockaddr		comm_socket; //input for accept; saves the communication socket for the connection requested
	socklen_t		addrlen;

	addrlen = sizeof(comm_socket);
	tmp = accept(this->_sockfd, &comm_socket, &addrlen);
	if (tmp < 0)
		std::cout << "Accept failed" << std::endl;
	else
	{
		this->_connection_fds.push_back(init_pollfd());
		this->_connection_fds[this->_amnt_connections].fd = tmp;
		this->_amnt_connections++;
		// add_member(); - check all necessary input e.g. PASS and NICK, etc so that User is only allowed if complete
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
	while (Server::_signal_status == false)
	{
		err = poll(&this->_connection_fds[0], this->_amnt_connections, -1);
		if (err <= -1)
			throw ClientConnectionFailed();
		for (int i = 0; i < this->_amnt_connections; i++)
		{
			if (this->_connection_fds[i].revents & (POLLIN | POLLHUP)) // is anywhere input waiting
			{
				if (this->_connection_fds[i].fd == this->_sockfd)
					this->accept_connection();
				else
					this->communicate(i);
			}
			if (this->_connection_fds[i].revents & POLLHUP)
			{
				if (this->_connection_fds[i].fd == this->_sockfd) // does this make sense? - if bind socket is gone server is off, right?
					break ;
				else
				{
					if (recv(this->_connection_fds[i].fd, test, sizeof(test), 0) == 0)
					{
						close (this->_connection_fds[i].fd);
						this->_connection_fds.erase(_connection_fds.begin() + i);
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
			if (setsockopt(test, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) // option to allow reusage of the same port without waiting incl keeping up connections
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
	if (input.empty()) {
		throw std::invalid_argument("Input cannot be empty.");
	} else if (SPACE.find(input[0]) != std::string::npos) {
		throw std::invalid_argument("Input cannot start with a space.");
	}

	std::string command;
	std::vector<std::string> args;
	std::istringstream stream(input);
	std::string word;

	if (stream >> command) {
		args.push_back(command);
		while (stream >> word) {
			args.push_back(word);
		}
	}
	return Request(command, args);
}

void Server::execute(Request request){
	if (request.getCommand() == Command::PASS) // TODO(KL)
		std::cout << "PASS" << std::endl;
	else
		throw std::invalid_argument("Invalid command.");
}

void	Server::signal_handler(int signal)
{
	(void)signal;
	Server::_signal_status = true;
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
