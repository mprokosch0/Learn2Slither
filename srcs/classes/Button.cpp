#include  "learn2Slither.hpp"

int Button::_borderWidth = 2;

Button::Button(box dim, int color, int hoverColor, int borderColor,
	int borderHoverColor, Text text, bool active,
	std::function<void()> func):
	_text(text), _dimentions(dim), _color(color), _hoverColor(hoverColor),
	_borderColor(borderColor), _borderHoverColor(borderHoverColor),
	_hover(false), _active(active), _onClickFunction(func)
{}

Button::Button(Button const &rhs): _text(rhs._text),
	 _dimentions(rhs._dimentions), _color(rhs._color),
	 _hoverColor(rhs._hoverColor), _borderColor(rhs._borderColor),
	 _borderHoverColor(rhs._borderHoverColor), _hover(rhs._hover),
	 _active(rhs._active), _onClickFunction(rhs._onClickFunction)
{}

Button::~Button()
{}

Button	&Button::operator=(Button const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_text = rhs._text;
	this->_dimentions = rhs._dimentions;
	this->_color = rhs._color;
	this->_hoverColor = rhs._hoverColor;
	this->_borderColor = rhs._borderColor;
	this->_borderHoverColor = rhs._borderHoverColor;
	this->_hover = rhs._hover;
	this->_active = rhs._active;
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
	if (!this->_active)
		return false;
	if (x >= this->_dimentions.x && x < this->_dimentions.x + this->_dimentions.w
		&& y >= this->_dimentions.y && y < this->_dimentions.y + this->_dimentions.h)
	{
		this->_hover = true;
		return true;
	}
	return false;
}

bool	Button::hoverIfOnButtonClip(int x, int y, box &clip)
{
	if (!this->_active)
		return false;
	if (x >= clip.x && x < clip.x + clip.w
		&& y >= clip.y && y < clip.y + clip.h)
	{
		this->_hover = true;
		return true;
	}
	return false;
}

void	Button::setDimentions(box &dim)
{
	this->_dimentions = dim;
}

const box	&Button::getDimentions(void) const
{
	return this->_dimentions;
}

const Text	&Button::getText(void) const
{
	return this->_text;
}

bool	Button::isActive(void) const
{
	return this->_active;
}

void	Button::activate(void)
{
	this->_active = true;
}

void	Button::desactivate(void)
{
	this->_active = false;
}

void	Button::click(void) const
{
	if (this->_active)
		this->_onClickFunction();
}

void	Button::render(Image &img) const
{
	if (!this->_active)
		return ;
	int borderColor = (this->_hover) ? this->_borderHoverColor : this->_borderColor;
	int color = (this->_hover) ? this->_hoverColor : this->_color;
	img.putRectangle(this->_dimentions, color);
	box border = this->_dimentions;
	border.h = this->_borderWidth;
	img.putRectangle(border, borderColor);
	border.y += (this->_dimentions.h - this->_borderWidth);
	img.putRectangle(border, borderColor);
	border = {.x = border.x, .y = this->_dimentions.y + this->_borderWidth,
				.w = this->_borderWidth,
				.h = this->_dimentions.h - (2 * this->_borderWidth)};
	img.putRectangle(border, borderColor);
	border.x += (this->_dimentions.w - this->_borderWidth);
	img.putRectangle(border, borderColor);
	Text::Colors col = (this->_hover) ? Text::Colors::Secondary : Text::Colors::Primary;
	this->_text.render(this->_dimentions.x + (this->_dimentions.w / 2),
						this->_dimentions.y + (this->_dimentions.h / 2),
					img, Text::HAlign::Center, Text::VAlign::Center, col);
}

void	Button::renderClip(Image &img, box &clip) const
{
	if (!this->_active)
		return ;
	int borderColor = (this->_hover) ? this->_borderHoverColor : this->_borderColor;
	int color = (this->_hover) ? this->_hoverColor : this->_color;
	img.putRectangle(clip, color);
	if (this->_dimentions.y >= clip.y)
	{
		box border = {clip.x, clip.y, clip.w, this->_borderWidth};
		img.putRectangle(border, borderColor);
	}
	if (this->_dimentions.y + this->_dimentions.h <= clip.y + clip.h)
	{
		box border = {clip.x, clip.y + clip.h - this->_borderWidth, clip.w, this->_borderWidth};
		img.putRectangle(border, borderColor);
	}
	box border = {clip.x, clip.y, this->_borderWidth, clip.h};
	img.putRectangle(border, borderColor);
	border.x += clip.w - this->_borderWidth;
	img.putRectangle(border, borderColor);
	Text::Colors col = (this->_hover) ? Text::Colors::Secondary : Text::Colors::Primary;
	this->_text.renderClip(this->_dimentions.x + (this->_dimentions.w / 2),
						this->_dimentions.y + (this->_dimentions.h / 2),
					img, clip, Text::HAlign::Center, Text::VAlign::Center, col);
}