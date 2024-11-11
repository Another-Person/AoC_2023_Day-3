// AoC_2023_Day-3.cpp : Defines the entry point for the application.
//

#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <sstream>

#include "AoC_2023_Day-3.h"

using std::string;
using std::cout;
using std::cerr;
using std::ifstream;
using std::exception;
using ss = std::stringstream;

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
	int retval = 0;

	string pathName = "./input.txt";

	if (argc == 2 && argv[1][0] != NULL)
	{
		cout << "Using input file from command line \"" << argv[1] << "\n";
		pathName = argv[1];
	}
	else
	{
		cout << "Using default file path " << pathName << "\n";
	}

	try
	{
		fs::path path = pathName;
		path.make_preferred();

		if (!fs::exists(path))
		{
			ss msg;
			msg << "File \"" << path.string() << "\" does not exist.\n";
			throw std::invalid_argument(msg.str());
		}

		if (!fs::is_regular_file(path))
		{
			ss msg;
			msg << "File \"" << path.string() << "\" is not a valid regular file.\n";
			throw std::invalid_argument(msg.str());
		}

		ifstream inputFile(path);

		if (!inputFile.is_open())
		{
			ss msg;
			msg << "Unable to open file \"" << path.string() << "\".\n";
			throw std::runtime_error(msg.str());
		}

		cout << "Displaying read data: \n";

		string line;
		while (std::getline(inputFile, line))
		{
			cout << line << "\n";
		}
		
	}
	catch (exception &e)
	{
		cerr << "Caught exception: " << e.what() << "\n";
		retval = 1;
	}
	catch (...)
	{
		cerr << "Unknown exception encountered.\n";
		retval = 1;
	}

	return retval;
}
