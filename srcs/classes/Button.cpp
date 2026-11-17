#include  "learn2Slither.hpp"

int Button::_borderWidth = 2;

Button::Button(void):
	_text(), _textX(), _ascend(true),
	_tPoint(std::chrono::steady_clock::time_point{}),
	_renderCount(), _dimentions(),
	_color(), _hoverColor(), _borderColor(), _borderHoverColor(),
	_pressedColor(), _pressedBorderColor(),
	_hover(), _active(), _pressed(), _onClickFunction()
{}

Button::Button(box dim, int color, int hoverColor, int borderColor,
	int borderHoverColor, int pressedColor, int pressedBorderColor,
	Text text, bool active, std::function<void()> func):
	_text(text), _textX(dim.x + this->_borderWidth), _ascend(true),
	_tPoint(std::chrono::steady_clock::time_point{}), _renderCount(0),
	_dimentions(dim), _color(color), _hoverColor(hoverColor),
	_borderColor(borderColor), _borderHoverColor(borderHoverColor),
	_pressedColor(pressedColor), _pressedBorderColor(pressedBorderColor),
	_hover(false), _active(active), _pressed(false), _onClickFunction(func)
{}

Button::~Button()
{}

void	Button::clearHover(void)
{
	this->_hover = false;
}

bool	Button::isPressed(void) const
{
	return this->_pressed;
}

void	Button::setPressed(bool press)
{
	this->_pressed = press;
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

void	Button::setText(std::string txt)
{
	this->_text.setText(txt);
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

void	Button::click(void)
{
	if (this->_active)
	{
		this->_onClickFunction();
		this->_pressed = true;
	}
}

# include <unistd.h>

void	Button::render(Image &img)
{
	if (!this->_active)
		return ;
	int borderColor = (this->_hover) ? this->_borderHoverColor : this->_borderColor;
	borderColor = (this->_pressed) ? this->_pressedBorderColor : borderColor;
	int color = (this->_hover) ? this->_hoverColor : this->_color;
	color = (this->_pressed) ? this->_pressedColor : color;
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
	Text::Colors col = (this->_hover || this->_pressed) ? Text::Colors::Secondary : Text::Colors::Primary;
	if ((int)this->_text.pxSize() > this->_dimentions.w)
	{
		box clip = this->_dimentions;
		clip.x += this->_borderWidth;
		clip.y += this->_borderWidth;
		clip.w -= this->_borderWidth * 2;
		clip.h -= this->_borderWidth * 2;

		const int textW = (int)this->_text.pxSize();
		const int maxX  = clip.x + 2;
		const int minX  = clip.x + clip.w - textW - 2;

		this->_text.renderClip(this->_textX, clip.y + (clip.h / 2),
						img, clip, Text::HAlign::Left, Text::VAlign::Center, col);

		const bool pause = (this->_tPoint != std::chrono::steady_clock::time_point{});

		if (pause)
		{
			std::chrono::duration<double> elapsed =
				std::chrono::steady_clock::now() - this->_tPoint;
			if (elapsed.count() >= 2.0)
			{
				this->_tPoint = std::chrono::steady_clock::time_point{};
				this->_ascend = !this->_ascend;
				this->_renderCount = 0;
			}
		}
		else
		{
			if (!(this->_renderCount % 10))
				this->_textX += this->_ascend ? -1 : 1;
			this->_renderCount++;

			if (this->_textX <= minX || this->_textX >= maxX)
			{
				this->_textX = std::clamp(this->_textX, minX, maxX);
				this->_tPoint = std::chrono::steady_clock::now();
			}
		}
		return ;
	}
	this->_text.render(this->_dimentions.x + (this->_dimentions.w / 2),
							this->_dimentions.y + (this->_dimentions.h / 2),
						img, Text::HAlign::Center, Text::VAlign::Center, col);
}

void	Button::renderClip(Image &img, box &clip)
{
	if (!this->_active)
		return ;
	int borderColor = (this->_hover) ? this->_borderHoverColor : this->_borderColor;
	borderColor = (this->_pressed) ? this->_pressedBorderColor : borderColor;
	int color = (this->_hover) ? this->_hoverColor : this->_color;
	color = (this->_pressed) ? this->_pressedColor : color;
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
	Text::Colors col = (this->_hover || this->_pressed) ? Text::Colors::Secondary : Text::Colors::Primary;
	if ((int)this->_text.pxSize() > this->_dimentions.w)
	{
		box cliped = clip;
		cliped.x += this->_borderWidth;
		cliped.y += this->_borderWidth;
		cliped.w -= this->_borderWidth * 2;
		cliped.h -= this->_borderWidth * 2;

		const int textW = (int)this->_text.pxSize();
		const int maxX  = cliped.x + 2;
		const int minX  = cliped.x + cliped.w - textW - 2;

		this->_text.renderClip(this->_textX, this->_dimentions.y + (this->_dimentions.h / 2),
						img, cliped, Text::HAlign::Left, Text::VAlign::Center, col);

		const bool pause = (this->_tPoint != std::chrono::steady_clock::time_point{});

		if (pause)
		{
			std::chrono::duration<double> elapsed =
				std::chrono::steady_clock::now() - this->_tPoint;
			if (elapsed.count() >= 2.0)
			{
				this->_tPoint = std::chrono::steady_clock::time_point{};
				this->_ascend = !this->_ascend;
				this->_renderCount = 0;
			}
		}
		else
		{
			if (!(this->_renderCount % 10))
				this->_textX += this->_ascend ? -1 : 1;
			this->_renderCount++;

			if (this->_textX <= minX || this->_textX >= maxX)
			{
				this->_textX = std::clamp(this->_textX, minX, maxX);
				this->_tPoint = std::chrono::steady_clock::now();
			}
		}
		return ;
	}
	this->_text.renderClip(this->_dimentions.x + (this->_dimentions.w / 2),
						this->_dimentions.y + (this->_dimentions.h / 2),
					img, clip, Text::HAlign::Center, Text::VAlign::Center, col);
}