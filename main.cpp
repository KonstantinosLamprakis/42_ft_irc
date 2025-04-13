int main (int ac, char **argv){
	if (ac != 3)
	{

	}
	try
	{

		server->start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

}
