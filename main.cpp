#include "headers/Server.hpp"

std::string validateInput(int ac, char **argv);

int main (int ac, char **argv){
	try
	{
		std::string error = validateInput(ac, argv);
		if (error != ""){
			std::cerr << error << std::endl;
			return 1;
		}
		Server	server(std::stoi(argv[1]), argv[2]);
		server.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
}

/**
 * @brief Validates that the initial arguments are in format: ./ircserv <port> <password>
 * 
 * @param ac number of initial args, taken directly from main
 * @param argv initial args, taken directly from main
 * @return std::string contains the error message in case of invalid input, empty in case of success
 */
std::string validateInput(int ac, char **argv){
	if (ac != 3){
		return "Error: expected 3 arguments. The format should be:\n./ircserv <port> <password>";
	}
	std::string portStr = argv[1];

	for (size_t i = 0; i < portStr.size(); ++i) {
		if (portStr[i] < '0' || portStr[i] > '9') return "Error: port should only contain digits";
    }

	if (portStr.size() > 5 || std::stoi(portStr) > 65535){
		return "Error: port should only be between 1 and 65535";
	}

	return "";
}
