#include "learn2Slither.hpp"

size_t	Text::_height = 14;

Text::Text(std::string text, float zoom, int color):
	_text(text), _zoom(zoom), _width(Image::getStringSize(text, zoom)),
	_color(color)
{}

Text::Text(Text const &rhs)
{
	*this = rhs;
}

Text::~Text()
{}

Text	&Text::operator=(Text const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_text = rhs._text;
	this->_zoom = rhs._zoom;
	this->_width = rhs._width;
	this->_color = rhs._color;
	return *this;
}

void	Text::render(int x, int y, Image &img, HAlign hAlign, VAlign vAlign)
{
	size_t newX = 0, newY = 0;
	switch (hAlign)
	{
		case HAlign::Left:
			newX = x;
			break;
		
		case HAlign::Center:
			newX = x - (this->_width / 2);
			break;
		
		case HAlign::Right:
			newX = x - this->_width;
			break;
	}
	switch (vAlign)
	{
		case VAlign::Top:
			newY = y;
			break;
		
		case VAlign::Center:
			newY = y - (this->_height / 2);
			break;
		
		case VAlign::Bottom:
			newY = y - this->_height;
			break;
	}
	img.putString(newX, newY, this->_text, this->_color, this->_zoom);
}