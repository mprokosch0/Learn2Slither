#include  "learn2Slither.hpp"

int Button::_borderWidth = 3;

Button::Button(box dim, int color): _dimentions(dim), _color(color), _hover(true)
{}

Button::Button(Button const &rhs): _dimentions(rhs._dimentions)
{}

Button::~Button()
{}

Button	&Button::operator=(Button const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_dimentions = rhs._dimentions;
	return *this;
}

void	Button::render(Image &img)
{
	img.putRectangle(this->_dimentions, this->_color);
	box border = this->_dimentions;
	border.h = this->_borderWidth;
	int color = (this->_hover) ? 0xffffff : 0x000000;
	img.putRectangle(border, color);
	border.y += (this->_dimentions.h - this->_borderWidth);
	img.putRectangle(border, color);
	border = {.x = border.x, .y = this->_dimentions.y + this->_borderWidth,
				.w = this->_borderWidth,
				.h = this->_dimentions.h - (2 * this->_borderWidth)};
	img.putRectangle(border, color);
	border.x += (this->_dimentions.w - this->_borderWidth);
	img.putRectangle(border, color);
}