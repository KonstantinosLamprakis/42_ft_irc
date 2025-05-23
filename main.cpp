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
		std::string err_msg = "Error: Expected 3 arguments.\nUsage: ./ircserv <port> <password>";
		std::cerr << err_msg << std::endl;
		throw std::runtime_error(err_msg);
	}

	std::string portStr = argv[1];
	for (size_t i = 0; i < portStr.size(); ++i) {
		if (portStr[i] < '0' || portStr[i] > '9') {
			std::string err_msg = "Error: Port should only contain digits.";
			std::cerr << err_msg << std::endl;
			throw std::runtime_error(err_msg) ;
		}
	}

	if (portStr.size() > 5 || std::stoi(portStr) > 65535 || std::stoi(portStr) < 1025){
		std::string err_msg = "Error: port should only be between 1024 and 65535";
		std::cerr << err_msg << std::endl;
		throw std::runtime_error(err_msg);
	}
}

int main (int ac, char **argv)
{
	struct sigaction	s;

	memset (&s.sa_mask, 0, sizeof (s.sa_mask));
	s.sa_handler = &(Server::signal_handler);
	s.sa_flags = 0;
	sigaction(SIGINT, &s, NULL);
	sigaction(SIGTERM, &s, NULL);
	sigaction(SIGQUIT, &s, NULL);
	try
	{
		validateInput(ac, argv);
		Server	irc_server(std::stoi(argv[1]), argv[2]);
		irc_server.start();
		irc_server.close_and_free_socket(std::string());
	}
	catch(const std::exception& e){
		return (1);
	}
	return (0);
}
