#include  "learn2Slither.hpp"

int Button::_borderWidth = 2;

Button::Button(box dim, int color, std::string text): _text(text),
	_textSize(Image::getStringSize(text)), _dimentions(dim), _color(color),
	_hover(false)
{}

Button::Button(Button const &rhs): _text(rhs._text), _textSize(rhs._textSize),
	 _dimentions(rhs._dimentions), _color(rhs._color), _hover(rhs._hover)
{}

Button::~Button()
{}

Button	&Button::operator=(Button const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_dimentions = rhs._dimentions;
	this->_color = rhs._color;
	this->_hover = rhs._hover;
	this->_text = rhs._text;
	this->_textSize = rhs._textSize;
	return *this;
}

void	Button::clearHover(void)
{
	this->_hover = false;
}

bool	Button::hoverIfOnButton(int x, int y)
{
	if (x >= this->_dimentions.x && x < this->_dimentions.x + this->_dimentions.w
		&& y >= this->_dimentions.y && y < this->_dimentions.y + this->_dimentions.h)
	{
		this->_hover = true;
		return true;
	}
	return false;
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
	img.putString(this->_dimentions.x + (this->_dimentions.w / 2)
					- (this->_textSize / 2),
					this->_dimentions.y + (this->_dimentions.h / 2) - 7, this->_text);
}