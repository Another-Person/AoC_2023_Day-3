/* AoC 2023 Day 3
 * Joey Sachtleben
 *
 * An attempted solution
 */

#include <algorithm>
#include <cctype>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "Node.h"

namespace fs = std::filesystem;

using std::cerr;
using std::cout;
using std::ranges::for_each;
using std::isdigit;
using std::ispunct;
using std::invalid_argument;
using std::string;
using std::string_view;
using std::vector;

using NodeRow = vector<Node>;
using NodeGrid = vector<NodeRow>;
using ss = std::stringstream;



// Helper structs for counting Nodes
struct Count
{
	int sum {0};
	int validSum {0};
	void operator()(const Node& n)
	{
		sum++;
		if (n.isNearSpecial())
		{
			validSum++;
		}
	}
};
struct RowCount
{
	int sum {0};
	int validSum {0};
	void operator()(const NodeRow& row)
	{
		auto r = for_each(row, Count()).fun;
		sum += r.sum;
		validSum += r.validSum;
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

/* FindValidDigits
 *  Takes a NodeGrid and marks all Nodes which are near a symbol.
 *
 * Parameters:
 *	grid - A populated NodeGrid.
 *
 * Returns:
 *  Nothing. The NodeGrid is modified in place.
 *
 * Throws:
 *  May throw out_of_range.
 */
void FindValidDigits(NodeGrid& grid)
{
	cout << "Finding valid digits... ";
	cout.flush();

	for (size_t x = 0; x < grid.size(); ++x)
	{
		for (size_t y = 0; y < grid.at(x).size(); ++y)
		{
			// Validate that we're at a digit before processing
			Node& node = grid.at(x).at(y);
			if (!isdigit(node.getCharacter()))
			{
				continue;
			}

			// Go through and check all surrounding nodes to see if they have a special character (AKA punctuation other than .)
			// If they do, mark the node and move on to the next.

			if (x != 0)
			{
				if (y != 0)
				{
					const char c = grid.at(x-1).at(y-1).getCharacter();
					if (ispunct(c) && c != '.')
					{
						node.setNearSpecial();
						continue;
					}
				}

				const char c = grid.at(x-1).at(y).getCharacter();
				if (ispunct(c) && c != '.')
				{
					node.setNearSpecial();
					continue;
				}

				if (y != grid.at(x).size() -1)
				{
					const char c = grid.at(x-1).at(y+1).getCharacter();
					if (ispunct(c) && c != '.')
					{
						node.setNearSpecial();
						continue;
					}
				}
			}

			// braces here PURELY for the ability to collapse them in VS
			// and to make this block line up with the others better I guess
			{
				if (y != 0)
				{
					const char c = grid.at(x).at(y-1).getCharacter();
					if (ispunct(c) && c != '.')
					{
						node.setNearSpecial();
						continue;
					}
				}

				if (y != grid.at(x).size() - 1)
				{
					const char c = grid.at(x).at(y+1).getCharacter();
					if (ispunct(c) && c != '.')
					{
						node.setNearSpecial();
						continue;
					}
				}
			}

			if (x != grid.size() -1)
			{
				if (y != 0)
				{
					const char c = grid.at(x+1).at(y-1).getCharacter();
					if (ispunct(c) && c != '.')
					{
						node.setNearSpecial();
						continue;
					}
				}

				const char c = grid.at(x+1).at(y).getCharacter();
				if (ispunct(c) && c != '.')
				{
					node.setNearSpecial();
					continue;
				}

				if (y != grid.at(x).size() -1)
				{
					const char c = grid.at(x+1).at(y+1).getCharacter();
					if (ispunct(c) && c != '.')
					{
						node.setNearSpecial();
						continue;
					}
				}
			}
		}
	}

	cout << "done.\n";
}

/* FindValidNumbers
 *   Finds all valid numbers within a NodeGrid.
 * Parameters:
 *	grid - A populated NodeGrid. Must have been processed by FindValidDigits.
 * Returns:
 *	Nothing. The NodeGrid is modified in place.
 * Throws:
 *	May throw out_of_range.
 */
void FindValidNumbers(NodeGrid& grid)
{
	cout << "Finding valid numbers... ";
	cout.flush();

	for (size_t x = 0; x < grid.size(); ++x)
	{
		for (size_t y = 0; y < grid.at(x).size(); ++y)
		{
			// Find a node which *IS* near a special character but has *NOT* been already used in a number.
			// If either of these conditions is not met, move on to the next node.
			Node& node = grid.at(x).at(y);
			if (!node.isNearSpecial() || node.hasBeenUsed())
			{
				continue;
			}

			// Since we now know we are at a digit that's part of a valid number, mark it as such...
			node.use();

			// ...and then proceed to find any digits that are to the left or right of this digit and mark them as being part of a number.
			for (int r_y = y-1; r_y >= 0; --r_y)
			{
				Node& back = grid.at(x).at(r_y);
				if (isdigit(back.getCharacter()))
				{
					back.use();
				}
				else
				{
					break;
				}
			}
			for (size_t r_y = y+1; r_y < grid.at(x).size(); ++r_y)
			{
				Node& front = grid.at(x).at(r_y);
				if (isdigit(front.getCharacter()))
				{
					front.use();
				}
				else
				{
					break;
				}
			}

		}
	}

	cout << "done.\n";
}

/* CreateValidNumbers
 *	Creates valid numbers from a NodeGrid.
 * Parameters:
 *	grid - A populated NodeGrid which has been processed by FindValidNumbers.
 *	numbers - An empty vector of integers to be filled.
 * Returns:
 *	Nothing. numbers will be filled in place.
 * Throws:
 * May throw out_of_range or invalid_argument
 */
void CreateValidNumbers(const NodeGrid& grid, vector<int>& numbers)
{
	cout << "Creating valid numbers... ";
	cout.flush();

	for (size_t x = 0; x < grid.size(); ++x)
	{
		for (size_t y = 0; y < grid.at(x).size(); ++y)
		{
			// Find a digit that's marked as being in a number.
			// Since we're iterating left to right, top to bottom, we know that we'll always hit the first digit, well, first.
			if (!grid.at(x).at(y).hasBeenUsed())
			{
				continue;
			}

			// Starting at that digit, go through and find all connected digits to add them to a string
			string rawNum;
			while (y < grid.at(x).size())
			{
				const Node& node = grid.at(x).at(y);
				if (node.hasBeenUsed())
				{
					rawNum += node.getCharacter();
				}
				else
				{
					break;
				}
				y++;
			}

			// Add the resulting number to the vector
			numbers.emplace_back(std::stoi(rawNum));
		}
	}

	cout << "done.\n";
}

/* CreateNodes
 *	Fills a NodeGrid with nodes from a specified path.
 *
 * Parameters:
 *	path - a path to a file to be read. It is assumed the path has been validated by ValidatePath.
 *	nodes - an empty NodeGrid to be filled.
 *
 * Returns:
 *	Nothing. The NodeGrid is modified in place.
 *
 * Throws:
 *	runtime_error if unable to properly open path.
 *	May throw out_of_range.
 */
void CreateNodes(const fs::path& path, NodeGrid& nodes)
{
	std::ifstream inputFile(path);

	if (!inputFile.is_open())
	{
		ss msg;
		msg << "Unable to open file \"" << path.string() << "\".\n";
		throw std::runtime_error(msg.str());
	}

	nodes.emplace_back();
	while (!inputFile.eof())
	{
		const char c = static_cast<char>(inputFile.get());

		if (c == std::char_traits<char>::eof())
		{
			break;
		}

		// Only add a new row if this isn't the end of the file
		if (c == '\n')
		{
			if (inputFile.peek() != std::char_traits<char>::eof())
			{
				nodes.emplace_back();
			}
			continue;
		}
		nodes.at(nodes.size()-1).emplace_back(c);
	}
}

int main(int argc, char* argv[])
{
	int retval = 0;

	string pathName = "./input.txt";

	if (argc == 2 && argv[1][0] != NULL)
	{
		cout << "Using input file from command line \"" << argv[1] << "\".\n";
		pathName = argv[1];
	}
	else
	{
		cout << "Using default file path \"" << pathName << "\".\n";
	}

	try
	{
		fs::path path = ValidatePath(pathName);
		NodeGrid nodes;

		CreateNodes(path, nodes);
		FindValidDigits(nodes);

		const auto [nodesTotal, validSum] = for_each(nodes, RowCount()).fun;

		cout << "Created " << nodesTotal << " nodes in " << nodes.size() << " rows!\n";
		cout << "Found " << validSum << " digits near a special character!\n";

		FindValidNumbers(nodes);
		vector<int> numbers;
		CreateValidNumbers(nodes, numbers);
		int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
		cout << "Found " << numbers.size() << " valid numbers totalling to " << sum << "!\n";
		
	}
	catch (std::exception &e)
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
