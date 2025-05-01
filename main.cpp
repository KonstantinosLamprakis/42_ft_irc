#include "./headers/Server.hpp"
#include "./headers/Exceptions.hpp"
#include "./headers/Request.hpp"

/**
 * @brief Validates that the initial arguments are in format: ./ircserv <port> <password>
 * 
 * @param ac number of initial args, taken directly from main
 * @param argv initial args, taken directly from main
 * @return throws an exceptions in case of invalid input
 */
void validateInput(int ac, char **argv){
	if (ac != 3){
		throw std::runtime_error("Expected 3 arguments.\nUsage: ./ircserv <port> <password>");
	}

	std::string portStr = argv[1];
	for (size_t i = 0; i < portStr.size(); ++i) {
		if (portStr[i] < '0' || portStr[i] > '9') throw std::runtime_error("Port should only contain digits.") ;
	}

	if (portStr.size() > 5 || std::stoi(portStr) > 65535 || std::stoi(portStr) < 1025){
		throw std::runtime_error("Error: port should only be between 1024 and 65535");
	}
}

int main (int ac, char **argv)
{
	struct sigaction	s;

	memset (&s, 0, sizeof (s));
	s.sa_handler = (Server::signal_handler);
	try
	{
		sigaction(SIGINT, &s, NULL);
		sigaction(SIGQUIT, &s, NULL);
		validateInput(ac, argv);
		Server	irc_server(std::stoi(argv[1]), argv[2]);
		irc_server.start();
		irc_server.close_and_free_socket(std::string());
	}
	catch(const std::exception& e){
		std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);
}
