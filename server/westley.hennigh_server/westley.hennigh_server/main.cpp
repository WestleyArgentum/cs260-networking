// Westley Hennigh
// main.cpp: The main for the client
// CS260 Assignment 2
// Feb 22th 2010

#include <sstream>
#include <iostream>
#include <algorithm>

#include "Server.hpp"

extern const unsigned BUFF_SIZE;

bool ReportErrorAndPromptToQuit(int error);  // forward declare function below

int main(void)
{
	std::string command_line_buffer;  // for grabbing data from the command line as we set things up

	int ret = 0;
	
	// get a port -----
	std::cout << "Specify a port: ";
	std::getline(std::cin, command_line_buffer);
	// --------

	// convert port_str from a string to an unsigned ------
	std::stringstream converter;
	unsigned port;
	converter << command_line_buffer;
	converter >> port;
	// --------

	// make a server, tell it to run -----
	Server tehServer(port);
	ret = tehServer.Run();
	
	// while that does it's thing, lets look at the command line for quit
	std::getline(std::cin, command_line_buffer);
	std::transform(command_line_buffer.begin(), command_line_buffer.end(), command_line_buffer.begin(), toupper);
	while (command_line_buffer != "QUIT")
	{
		std::getline(std::cin, command_line_buffer);
		std::transform(command_line_buffer.begin(), command_line_buffer.end(), command_line_buffer.begin(), toupper);
	}

	// now that we received quit, shutdown and return from main
	ret = tehServer.Shutdown();
	// ----------

	return 0;
}

bool ReportErrorAndPromptToQuit(int error)
{
	// report the error and ask if they want to quit (return of true = yes)
	std::cout << "Error setting up client: " << error << std::endl;
	std::cout << "Try again? Or quit?" << std::endl;

	std::string command_line_buffer;
	std::getline(std::cin, command_line_buffer);
	std::transform(command_line_buffer.begin(), command_line_buffer.end(), command_line_buffer.begin(), toupper);
	if ( command_line_buffer == "QUIT" )
		return true;
	else
	{
		std::cout << std::endl << "^.^" << std::endl << std::endl;
		return false;
	}
}
