#include "../headers/Server.hpp"
#include "../headers/Exceptions.hpp"
#include <sstream>

Server::Server()
{
	memset(this->_connection_fds, 0, MAX_CONNECTIONS);
}

Server::Server() : _port(0), _password("") 
{
	memset(this->_connection_fds, 0, MAX_CONNECTIONS);
}

Server::Server(Server &copy): _port(copy._port), _password(copy._password){}

Server &Server::operator=(Server &old){
    this->_port = old._port;
    this->_password = old._password;    
    return (*this);
}

Server::~Server() {}

Server::Server(int port, std::string password){
	this->_port = port;
	this->_password = password;
}

/**
 * @brief initializes User and handles recv and send for the individual connection?! - need to think about that again
 * 
 * @param User each client gets own user incl. fd and data
 * @return "running 1 on 1 connection"
 */

void	Server::run_connection(int fd)
{
	User User(fd);

	//here the class needs to be filled with the input data from the "login connection to the server"
}

/**
 * @brief listens to the open socket of the server for incoming clients
 * 
 * @param tmp temporary storage of the fd for the new connection
 * @param comm_socket input for accept; saves the communication socket for the connection requested
 * @param i iterator
 * @return "running connection"
 */

void	Server::listentosocket() //not directly throw error - server should keep running
{
	int			tmp;
	sockaddr	comm_socket;
	int			i = 0;

	if (listen(this->_sockfd, BACKLOG) != 0)
	{
		std::cout << "Listen failed" << std::endl;
		// throw ClientConnectionFailed() ;
	}
	this->_connection_fds[0].fd = this->_sockfd;
	this->_connection_fds[0].events = 0;
	this->_connection_fds[0].events = POLLIN;
	this->_connection_fds[0].revents = 0;
	if (tmp = accept(this->_sockfd, &comm_socket, (unsigned int *)sizeof(comm_socket)) == -1)
	{
		std::cout << "Accept failed" << std::endl;
		// throw ClientConnectionFailed() ;
	}
	else
	{
		// if (this->_size_pollfd_struct < MAX_CONNECTIONS)
		// {
		// 	this->_connection_fds[this->_size_pollfd_struct].fd = tmp;
		// 	this->_size_pollfd_struct++;
		// }
		// else
		// {
		// 	std::cout << "The maximum amount of connections is reached" << std::endl;
		// 	return ;
		// }
		this->run_connection(tmp);
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

void	Server::start(){
	struct	addrinfo	server_hints;
	struct	addrinfo*	tmp;
	int					test;
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
		if (test = socket(this->_server_info->ai_family, this->_server_info->ai_socktype, this->_server_info->ai_protocol) == -1)
		{
			continue ;
		}
		else
		{
			if (setsockopt(test, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1); // option to allow reusage of the same port without waiting incl keeping up connections
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
		close_and_free_socket(NULL);
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
		close_and_free_socket(NULL);
		// freeaddrinfo(this->_server_info);
		throw e ;
	}
	close_connections();
	close_and_free_socket(NULL);
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

void	Server::close_and_free_socket(std::string err_msg)
{
	if (err_msg.empty())
		std::cout << err_msg << std::endl;
	close(this->_sockfd);
	freeaddrinfo(this->_server_info);
}

void	Server::close_connections()
{
	int	i = 1;

	while (i < this->_size_pollfd_struct && this->_connection_fds[i].fd != 0)
		close (this->_connection_fds[i++].fd);
}
