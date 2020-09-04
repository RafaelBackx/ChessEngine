#ifndef POSITION_H
#define POSITION_H
#include <iostream>

struct Position
{
	int x, y;
	Position(int x,int y): x(x),y(y){}
	Position() : x(0),y(0) {}
	friend std::ostream& operator<<(std::ostream& output, const Position& pos)
	{
		output << "(" << pos.x << "," << pos.y << ")";
		return output;
	}
};

#endif // !POSITION_H

