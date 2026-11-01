#include  "learn2Slither.hpp"

int Button::_borderWidth = 2;

Button::Button(box dim, int color, int hoverColor, int borderColor,
	int borderHoverColor, Text text, std::function<void(Engine &)> func):
	_text(text), _dimentions(dim), _color(color), _hoverColor(hoverColor),
	_borderColor(borderColor), _borderHoverColor(borderHoverColor),
	_hover(false), _onClickFunction(func)
{}

Button::Button(Button const &rhs): _text(rhs._text),
	 _dimentions(rhs._dimentions), _color(rhs._color),
	 _hoverColor(rhs._hoverColor), _borderColor(rhs._borderColor),
	 _borderHoverColor(rhs._borderHoverColor), _hover(rhs._hover),
	 _onClickFunction(rhs._onClickFunction)
{}

Button::~Button()
{}

Button	&Button::operator=(Button const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_dimentions = rhs._dimentions;
	this->_color = rhs._color;
	this->_hoverColor = rhs._hoverColor;
	this->_borderColor = rhs._borderColor;
	this->_borderHoverColor = rhs._borderHoverColor;
	this->_hover = rhs._hover;
	this->_text = rhs._text;
	this->_onClickFunction = rhs._onClickFunction;
	return *this;
}

void	Button::clearHover(void)
{
	this->_hover = false;
}

bool	Button::isMouseOn(void) const
{
	return this->_hover;
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

void	Button::click(Engine &engine) const
{
	this->_onClickFunction(engine);
}

void	Button::render(Image &img)
{
	img.putRectangle(this->_dimentions, this->_color);
	box border = this->_dimentions;
	border.h = this->_borderWidth;
	int color = (this->_hover) ? this->_borderHoverColor : this->_borderColor;
	img.putRectangle(border, color);
	border.y += (this->_dimentions.h - this->_borderWidth);
	img.putRectangle(border, color);
	border = {.x = border.x, .y = this->_dimentions.y + this->_borderWidth,
				.w = this->_borderWidth,
				.h = this->_dimentions.h - (2 * this->_borderWidth)};
	img.putRectangle(border, color);
	border.x += (this->_dimentions.w - this->_borderWidth);
	img.putRectangle(border, color);
	this->_text.render(this->_dimentions.x + (this->_dimentions.w / 2),
						this->_dimentions.y + (this->_dimentions.h / 2),
					img, Text::HAlign::Center, Text::VAlign::Center);
}