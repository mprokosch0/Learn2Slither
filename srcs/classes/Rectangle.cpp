#include "learn2Slither.hpp"

Rectangle::Rectangle(void): _dimentions((box){0, 0, 0, 0}), _color(0xffffff),
	_borderWidth(0), _borderColor(0xffffff)
{}

Rectangle::Rectangle(box dims, int color, size_t borderWidth, int borderColor):
	_dimentions(dims), _color(color), _borderWidth(borderWidth),
	_borderColor(borderColor)
{}

Rectangle::Rectangle(Rectangle const &rhs): _dimentions(rhs._dimentions),
	_color(rhs._color), _borderWidth(rhs._borderWidth),
	_borderColor(rhs._borderColor)
{}

Rectangle::~Rectangle()
{}

Rectangle	&Rectangle::operator=(Rectangle const &rhs)
{
	if (this == &rhs)
		return *this;

	this->_dimentions = rhs._dimentions;
	this->_color = rhs._color;
	this->_borderWidth = rhs._borderWidth;
	this->_borderColor = rhs._borderColor;
	return *this;
}

box		Rectangle::getDimentions(void) const
{
	return this->_dimentions;
}

void	Rectangle::render(Image &img) const
{
	img.putRectangleBorder(this->_dimentions, this->_color,
		this->_borderWidth, this->_borderColor);
}