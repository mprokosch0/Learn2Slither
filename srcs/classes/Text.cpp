#include "learn2Slither.hpp"

size_t	Text::_height = 14;

Text::Text(void): _text(""), _zoom(1.f),
	_width(0),
	_color(0xffffff), _hoverColor(0xffffff)
{}

Text::Text(std::string text, float zoom, int color, int secondCol):
	_text(text), _zoom(zoom), _width(Image::getStringSize(text, zoom)),
	_color(color), _hoverColor(secondCol)
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
	this->_hoverColor = rhs._hoverColor;
	return *this;
}

const std::string	&Text::getText(void) const
{
	return this->_text;
}

void	Text::render(int x, int y, Image &img, HAlign hAlign,
	VAlign vAlign, Colors color) const
{
	size_t newX = 0, newY = 0;
	int col = this->_color;
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
	if (color == Colors::Secondary)
		col = this->_hoverColor;

	img.putString(newX, newY, this->_text, col, this->_zoom);
}

void	Text::renderClip(int x, int y, Image &img, box &clip, HAlign hAlign,
	VAlign vAlign, Colors color) const
{
	size_t newX = 0, newY = 0;
	int col = this->_color;
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
	if (color == Colors::Secondary)
		col = this->_hoverColor;

	img.putStringClip(newX, newY, this->_text, clip, col, this->_zoom);
}