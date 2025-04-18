#include "../headers/Server.hpp"
#include "../headers/Exceptions.hpp"

Server::Server()
{
	memset(this->_connection_fds, 0, MAX_CONNECTIONS);
}

Server::Server() : _port(0), _password("") 
{
	memset(this->_connection_fds, 0, MAX_CONNECTIONS);
}

Server::Server(Server &copy){
	// if server is ready -> fill the copy constructor
}

Server& Server::operator=(Server &old){
	//if server is ready -> fill the assignement operator
}

Server::~Server() {}

Server::Server(int port, std::string password){
	this->_port = port;
	this->_password = password;
}

/**
 * @brief starts server and initializes ports
 * 
 * @param tmp temporary storage of the fd for the new connection
 * @param comm_socket input for accept; saves the communication socket for the connection requested
 * @return "running connection"
 */

void	Server::listentosocket() //not directly throw error - server should stay running
{
	int			tmp;
	sockaddr	comm_socket;

	if (listen(this->_sockfd, BACKLOG) != 0)
	{
		std::cout << "Listen failed" << std::endl;
		// throw ClientConnectionFailed() ;
	}
	if (tmp = accept(this->_sockfd, &comm_socket, (unsigned int *)sizeof(comm_socket)) == -1)
	{
		std::cout << "Accept failed" << std::endl;
		// throw ClientConnectionFailed() ;
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
 * @param test_2 the existing sockets get tested - if one of them works we continue with it 
 * @param yes value to set flag in setsockopt function (refered to as address)
 * @param i iterator
 * @return "running connection"
 */

void	Server::start(){
	struct	addrinfo	server_hints;
	struct	addrinfo*	tmp;
	int					test;
	int					test_2;
	int					yes = 1;
	int					i;

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
			freeaddrinfo(this->_server_info);
			// throw ServerConnectionFailed();
			continue ;
		}
		else
		{
			setsockopt(test, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)); // option to allow reusage of the same port without waiting incl keeping up connections
			if (test_2 = bind(test, this->_server_info->ai_addr, this->_server_info->ai_addrlen) != 0)
			{
				freeaddrinfo(this->_server_info);
				// throw ServerConnectionFailed();
				continue ;
			}
			this->_sockfd = test;
			break ;
		}
		close(this->_sockfd);
		freeaddrinfo(this->_server_info);
		throw ServerConnectionFailed();
	}
	try
	{
		this->listentosocket();
	}
	catch(const std::exception &e)
	{
		freeaddrinfo(this->_server_info);
		throw e ;
	}
	close(this->_sockfd);
	i = 0;
	while (i < 1024 && this->_connection_fds[i] != NULL)
		close (this->_connection_fds[i++]);
	std::cout << "starting at port: " << this->_port << std::endl;
	freeaddrinfo(this->_server_info);
}

Request Server::parse(std::string input){
	std::cout << input;
	return Request();
}

void Server::execute(Request request){
	request = Request();
}
