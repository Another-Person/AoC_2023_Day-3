/* AoC 2023 Day 3
 * Joey Sachtleben
 *
 * An attempted solution
 */

#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "Node.h"

namespace fs = std::filesystem;

using std::cout;
using std::cerr;
using std::exception;
using std::ranges::for_each;
using std::ifstream;
using std::invalid_argument;
using std::string;
using std::string_view;

using NodeRow = std::vector<Node>;
using NodeGrid = std::vector<NodeRow>;
using ss = std::stringstream;



// Helper structs for counting Nodes
struct Count
{
	int sum {0};
	void operator()(const Node& n) { sum++; }
};
struct RowCount
{
	int sum {0};
	void operator()(const NodeRow& row)
	{
		sum += for_each(row, Count()).fun.sum;
	}
};

/* ValidatePath
 *   Creates a path to a specified file and validates that it is a reachable regular file.
 *
 * Parameters:
 *   pathName - A string containing the path to the desired file.
 *
 * Returns:
 *   A path to the file.
 *
 * Throws:
 *   invalid_argument if the file does not exist or is not a regular file.
 *
 */
fs::path ValidatePath(const string_view pathName)
{
	fs::path path = pathName;
	path.make_preferred();

	if (!fs::exists(path))
	{
		ss msg;
		msg << "File \"" << path.string() << "\" does not exist.\n";
		throw invalid_argument(msg.str());
	}

	if (!fs::is_regular_file(path))
	{
		ss msg;
		msg << "File \"" << path.string() << "\" is not a valid regular file.\n";
		throw invalid_argument(msg.str());
	}

	return path;
}

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
		fs::path path = ValidatePath(pathName);

		ifstream inputFile(path);

		if (!inputFile.is_open())
		{
			ss msg;
			msg << "Unable to open file \"" << path.string() << "\".\n";
			throw std::runtime_error(msg.str());
		}

		NodeGrid nodes;
		nodes.emplace_back();
		while (!inputFile.eof())
		{
			const char c = static_cast<char>(inputFile.get());

			if (c == -1)
			{
				break;
			}

			if (c == '\n')
			{
				nodes.emplace_back();
				continue;
			}
			nodes.at(nodes.size()-1).emplace_back(c);
		}

		const auto total = std::ranges::for_each(nodes, RowCount()).fun;

		cout << "Created " << total.sum << " nodes in " << nodes.size() << " rows!\n";
		
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
