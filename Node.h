#ifndef JS_AOC_2023_DAY3_NODE_H
#define JS_AOC_2023_DAY3_NODE_H

/* Node.h
 * Joey Sachtleben
 */


/* Node
 * Holds a character and a marker to identify if this node is near a special character in another node.
 */
class Node
{
public:
	Node(const char chr) : character(chr), nearSpecial(false) {};
	~Node() = default;

	char getCharacter() const { return character; }
	bool isNearSpecial() const { return nearSpecial; }
	void setNearSpecial() { nearSpecial = true; }


private:
	const char character;
	bool nearSpecial;
};

#endif // !JS_AOC_2023_DAY3_NODE_H
