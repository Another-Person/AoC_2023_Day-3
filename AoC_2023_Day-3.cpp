/* AoC 2023 Day 3
 * Joey Sachtleben
 *
 * An attempted solution
 */

#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Node.h"

namespace fs = std::filesystem;

using std::string;
using std::cout;
using std::cerr;
using std::ifstream;
using std::exception;
using std::ranges::for_each;

using ss = std::stringstream;
using NodeRow = std::vector<Node>;
using NodeGrid = std::vector<NodeRow>;


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
