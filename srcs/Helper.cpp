#include "../headers/Helper.hpp"

static bool	signal_status = false; 

void	signal_handler(int signal)
{
	(void)signal;
	signal_status = true;
}
