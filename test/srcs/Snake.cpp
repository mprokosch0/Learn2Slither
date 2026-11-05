#include "test.hpp"

std::array<std::array<Directions, 3>, 4>	Snake::_orienTable = 
	{{{Directions::NORTH, Directions::WEST, Directions::EAST},
	 {Directions::EAST, Directions::NORTH, Directions::SOUTH},
	 {Directions::SOUTH, Directions::EAST, Directions::WEST},
	 {Directions::WEST, Directions::SOUTH, Directions::NORTH}}};

Snake::Snake(size_t size): _initSize(size)
{
	this->_pos.reserve(size);
}

Snake::Snake(Snake const &rhs): _pos(rhs._pos), _initSize(rhs._initSize),
	_orientation(rhs._orientation)
{}

Snake::~Snake()
{}

Snake	&Snake::operator=(Snake const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_pos = rhs._pos;
	this->_initSize = rhs._initSize;
	this->_orientation = rhs._orientation;
	return *this;
}

void	Snake::move(Actions act)
{
	if (this->_pos.empty())
		return ;
	for (size_t i = this->_pos.size() - 1; i > 0; i--)
		this->_pos[i] = this->_pos[i - 1];
	this->_orientation = this->_orienTable[static_cast<size_t>(this->_orientation)]
												[static_cast<size_t>(act)];
	if (this->_orientation == Directions::NORTH)
		this->_pos[0].second -= 1;
	else if (this->_orientation == Directions::EAST)
		this->_pos[0].first += 1;
	else if (this->_orientation == Directions::SOUTH)
		this->_pos[0].second += 1;
	else if (this->_orientation == Directions::WEST)
		this->_pos[0].first -= 1;
}

void	Snake::determineOrient(void)
{
	if (this->_pos.size() == 0)
		return ;
	if (this->_pos.size() == 1)
		this->_orientation = Directions::NORTH;
	else
	{
		std::pair<int, int>	head = this->_pos[0];
		std::pair<int, int> neck = this->_pos[1];
		int xdiff = head.first - neck.first;
		if (xdiff > 0)
			this->_orientation = Directions::EAST;
		else if (xdiff < 0)
			this->_orientation = Directions::WEST;
		else
		{
			int ydiff = head.second - neck.second;
			if (ydiff > 0)
				this->_orientation = Directions::SOUTH;
			else if (ydiff < 0)
				this->_orientation = Directions::NORTH;
		}
	}
}

void	Snake::addSegment(int x, int y)
{
	this->_pos.push_back(std::make_pair(x, y));
}

void	Snake::removeSegment(void)
{
	this->_pos.pop_back();
}

size_t	Snake::size(void) const
{
	return this->_pos.size();
}

const std::pair<int, int>	&Snake::getHeadPos(void) const
{
	return this->_pos[0];
}

const std::pair<int, int>	&Snake::getTailPos(void) const
{
	return this->_pos.back();
}

const std::vector<std::pair<int, int>>	&Snake::getPos(void) const
{
	return this->_pos;
}

size_t	Snake::getInitSize(void) const
{
	return this->_initSize;
}

const Directions	&Snake::getOrientation(void) const
{
	return this->_orientation;
}

void	Snake::reset(void)
{
	this->_pos.clear();
}