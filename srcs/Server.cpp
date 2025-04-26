#include "../headers/Server.hpp"
#include "../headers/Exceptions.hpp"
#include "../headers/Helper.hpp"

bool Server::_signal_status = false;

Server::Server() : _port(0), _password("") 
{
	memset(this->_connection_fds, 0, MAX_CONNECTIONS * sizeof(_connection_fds[0]));
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		this->_connection_fds[i].fd = -1;
		this->_connection_fds[i].events = POLLIN & POLLHUP;
		this->_connection_fds[i].revents = 0;
	}
	this->_sockfd = -1;
	this->_server_info = NULL;
	this->_size_pollfd_struct = 0;
}

Server::Server(Server &copy): _port(copy._port), _password(copy._password){}

Server::~Server(){}

Server &Server::operator=(Server &old){
	this->_port = old._port;
	this->_password = old._password;
	return (*this);
}

Server::Server(int port, std::string password){
	this->_port = port;
	this->_password = password;
	memset(this->_connection_fds, 0, MAX_CONNECTIONS * sizeof(_connection_fds[0]));
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		this->_connection_fds[i].fd = -1;
		this->_connection_fds[i].events = POLLIN & POLLHUP;
		this->_connection_fds[i].revents = 0;
	}
	this->_sockfd = -1;
	this->_server_info = NULL;
	this->_size_pollfd_struct = 0;
}

// void	Server::send_data(Request in) //  can be done if request exists (sening msg to either channel or user or all)
// {
// 	(void)in;
// }

/**
 * @brief receives, reads and if necessary forwards message or executes command
 * 
 * @param buff temporary storage to recv data 
 * @param bytes_recvd return value of recv - amount of char read
 * @param str whole string - not yet sure whether necessary
 * @return "new fd_connections struct array which is added to the _onnection_fds in server"
 */

void	Server::communicate(int i)
{
	char		buff[BUFFER_SIZE];
	int			bytes_recvd = BUFFER_SIZE;
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
			this->_connection_fds[i].revents = 0;
			this->_connection_fds[i].fd = -1; // right now just extanding and not reducing fds - needs to be fixed with vector
			return ;
		}
		str = str + buff;
	}
	// Request in = parse(str);
	// execute_command; -> e.g. send_data(in);
}

/**
 * @brief accept connections to socket
 * 
 * @param tmp temporary storage of the fd for the new connection
 * @param comm_socket input for accept; saves the communication socket for the connection requested
 * @return "new fd_connections struct array which is added to the _onnection_fds in server"
 */

void	Server::accept_connection()
{
	int				tmp = 0;
	sockaddr		comm_socket;

	tmp = accept(this->_sockfd, &comm_socket, (unsigned int *)sizeof(comm_socket));
	if (tmp < 0)
		std::cout << "Accept failed" << std::endl;
	else
	{
		if (this->_size_pollfd_struct < MAX_CONNECTIONS)
		{
			this->_connection_fds[this->_size_pollfd_struct].fd = tmp;
			this->_size_pollfd_struct++;
			// add_member(); - check all necessary input e.g. PASS and NICK, etc so that User is only allowed if complete
		}
		else
		{
			std::cout << "The maximum amount of connections is reached" << std::endl;
			return ;
		}
	}
}

/**
 * @brief listens to the open socket of the server for incoming clients and requests
 * 
 * @param i iterator
 * @param err return value used for error handling
 * @return "running connection listening on _sockfd and adding client connections to connection_fds"
 */

void	Server::listentosocket() //not directly throw error - server should keep running
{
	int		err = 0;
	char	test[1024]; //std::string possible?

	if (listen(this->_sockfd, BACKLOG) != 0)
	{
		std::cout << "Listen failed" << std::endl;
		throw ClientConnectionFailed() ;
	}
	this->_connection_fds[0].fd = this->_sockfd;
	this->_connection_fds[0].events = 0;
	this->_connection_fds[0].events = POLLIN;
	this->_connection_fds[0].revents = 0;
	while (Server::_signal_status == false)
	{
		err = poll(&this->_connection_fds[0], this->_size_pollfd_struct, -1);
		if (err == -1)
			throw ClientConnectionFailed();
		else if (err == 0)
		{
			std::cout << "timeout" << std::endl; 
			throw ClientConnectionFailed();
		}
		for (int i = 0; i < this->_size_pollfd_struct; i++)
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
						close (this->_connection_fds[i].fd); //this way the array is not adapted - the size stays the same(has to be changed when using vector)
						this->_connection_fds[i].revents = 0;
						this->_connection_fds[i].fd = -1;
					}
				}
			}
		}
	}
}

/**
 * @brief starts server and initializes ports
 * 
 * @param _sockfd fd of the connection bind to port
 * @param _server_info input for getaddrinfo function - will contain "socket address structure"
 * @param server_hints input for getaddrinfo function - contains flags used to define server_info
 * @param tmp struct to check all generated versions in linked list which one is working
 * @param test all solutions in the linked list get tested - if one of them works we continue to try bind
 * @param yes value to set flag in setsockopt function (refered to as address) - probably not necessary and &1 would be enough
 * @return "running connection"
 */

void	Server::start()
{
	struct	addrinfo	server_hints;
	struct	addrinfo*	tmp;
	int					test = 0;
	int					yes = 1;

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
		this->_connection_fds[0].fd = this->_sockfd;
		this->_size_pollfd_struct = 1;
		this->listentosocket();
	}
	catch(const std::exception &e)
	{
		close_connections();
		close_and_free_socket(std::string());
		throw e ;
	}
}

Request Server::parse(std::string input) const {
	if (input.empty()) {
		throw std::invalid_argument("Input cannot be empty.");
	} else if (SPACE.find(input[0]) != std::string::npos) {
		throw std::invalid_argument("Input cannot start with a space.");
	} else if (input.size() >= 2 && input.substr(input.size() - 2) != CRLF) {
		throw std::invalid_argument("Input should end with \r\n characters.");
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
	if (err_msg.empty())
		std::cout << err_msg << std::endl;
	freeaddrinfo(this->_server_info);
	close(this->_sockfd);
}

void	Server::close_connections()
{
	int	i = 1;

	while (i < this->_size_pollfd_struct && this->_connection_fds[i].fd != -1)
		close (this->_connection_fds[i++].fd);
}
