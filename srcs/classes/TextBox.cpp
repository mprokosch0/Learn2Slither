#include "learn2Slither.hpp"

TextBox::TextBox(void): _box(), _input(), _cursorPos(0), _scrollX(0),
	_focused(false), _active(false), _inactiveColor(), _inactiveColorBorder(),
	_tPoint(std::chrono::steady_clock::time_point()), _mode(Mode::TEXT), _error(false)
{}

TextBox::TextBox(Text txt, Rectangle box, int inactiveColor, int inactiveColorBorder):
	_box(box), _input(txt), _cursorPos(0), _scrollX(0),
	_focused(false), _active(false),  _inactiveColor(inactiveColor),
	_inactiveColorBorder(inactiveColorBorder), _tPoint(std::chrono::steady_clock::time_point()),
	_mode(Mode::TEXT), _error(false)
{}

TextBox::~TextBox(void)
{}

bool	TextBox::isFocused(void) const
{
	return this->_focused;
}

bool	TextBox::isMouseOn(int x, int y) const
{
	if (!this->_active)
		return false;
	box d = this->_box.getDimentions();

	if (x >= d.x && x < d.x + d.w
		&& y >= d.y && y < d.y + d.h)
		return true;
	return false;
}

void	TextBox::setfocus(bool focus)
{
	this->_focused = focus;
}

void	TextBox::activate(void)
{
	this->_active = true;
}

void	TextBox::desactivate(void)
{
	this->_active = false;
}

void	TextBox::setColors(int textCol, int secTextCol,
			int rectCol, int rectBorderCol,
			int inactiveColor, int inactiveColorBorder)
{
	this->_input.setColors(textCol, secTextCol);
	this->_box.setColors(rectCol, rectBorderCol);
	this->_inactiveColor = inactiveColor;
	this->_inactiveColorBorder = inactiveColorBorder;
}

void	TextBox::setMode(Mode mode)
{
	this->_mode = mode;
}

std::string	TextBox::getText(void) const
{
	if (this->_active)
		return this->_input.getText();
	return "";
}

void	TextBox::setText(std::string txt)
{
	this->_input.setText(txt);
	this->_cursorPos = 0;
	this->_scrollX = 0;
}

unsigned int	TextBox::getUInt(void) const
{
	if (this->_mode == Mode::UNSIGNED_INT)
		return std::atoi(this->_input.getText().c_str());
	return 0;
}

static void keycodeConv(int &keycode)
{
	switch (static_cast<keycodes>(keycode))
	{
		case keycodes::ZERO:
			keycode = '0';
			break;
		
		case keycodes::ONE:
			keycode = '1';
			break;
		
		case keycodes::TWO:
			keycode = '2';
			break;
		
		case keycodes::THREE:
			keycode = '3';
			break;
		
		case keycodes::FOUR:
			keycode = '4';
			break;
		
		case keycodes::FIVE:
			keycode = '5';
			break;
		
		case keycodes::SIX:
			keycode = '6';
			break;
		
		case keycodes::SEVEN:
			keycode = '7';
			break;
		
		case keycodes::EIGHT:
			keycode = '8';
			break;
		
		case keycodes::NINE:
			keycode = '9';
			break;
	
		default:
			break;
	}
}

void	TextBox::doAction(int keycode)
{
	if (!this->_active)
		return ;
	keycodeConv(keycode);
	if (keycode < 256 && Image::isCharInFont(keycode))
	{
		switch (this->_mode)
		{
			case Mode::TEXT:
				this->_input.addChar(keycode, this->_cursorPos++);			
				break;

			case Mode::UNSIGNED_INT:
				if (std::isdigit(keycode))
				{
					this->_input.addChar(keycode, this->_cursorPos++);
					if (std::atoll(this->_input.getText().c_str()) > UINT32_MAX)
					{
						this->_input.setText(std::to_string(UINT32_MAX));
						this->_cursorPos = this->_input.size();
						this->_scrollX = this->_cursorPos;
					}
				}
				break;

			default:
				break;
		}
	}
	else if (keycode == ' ' && this->_mode != Mode::UNSIGNED_INT)
		this->_input.addChar(' ', this->_cursorPos++);
	else if (this->_cursorPos > 0 && static_cast<keycodes>(keycode) == keycodes::BACKSPACE)
		this->_input.deleteChar(this->_cursorPos-- - 1);
	else if (this->_cursorPos < this->_input.size() && static_cast<keycodes>(keycode) == keycodes::DEL)
		this->_input.deleteChar(this->_cursorPos);
	else if (static_cast<keycodes>(keycode) == keycodes::ARR_LEFT)
	{
		if (this->_cursorPos > 0)
			this->_cursorPos--;
	}
	else if (static_cast<keycodes>(keycode) == keycodes::ARR_RIGHT)
	{
		if (this->_cursorPos < this->_input.size())
			this->_cursorPos++;
	}
	else if (static_cast<keycodes>(keycode) == keycodes::HOME)
		this->_cursorPos = 0;
	else if (static_cast<keycodes>(keycode) == keycodes::END)
		this->_cursorPos = this->_input.size();
}

void TextBox::render(Image &img)
{
	box d = this->_box.getDimentions();
	const std::string &t = this->_input.getText();
	int bw  = this->_box.getBorderWidth();
	int pad = 2;

	d.x += bw + pad;
	d.y += bw;
	d.w -= (bw + pad) * 2;
	d.h -= bw * 2;

	if (this->_active)
		this->_box.render(img);
	else
		this->_box.render(img, this->_inactiveColor, this->_inactiveColorBorder);

	Text::Colors col = this->_active ? Text::Colors::Primary : Text::Colors::Secondary;
	float zoom  = this->_input.getZoom();
	int fullS = (int)this->_input.pxSize();
	int maxScroll = std::max(0, fullS - d.w);

	int anchorX, cursorX;
	Text::HAlign align;

	if (this->_mode != Mode::UNSIGNED_INT)
	{
		int cursorPx = Image::getStringSize(t.substr(0, this->_cursorPos), zoom);

		if (cursorPx - this->_scrollX < 0)
			this->_scrollX = cursorPx;
		else if (cursorPx - this->_scrollX > d.w)
			this->_scrollX = cursorPx - d.w;
		this->_scrollX = std::clamp(this->_scrollX, 0, maxScroll);

		align   = Text::HAlign::Left;
		anchorX = d.x - this->_scrollX;
		cursorX = d.x + cursorPx - this->_scrollX;
	}
	else
	{
		int toEnd = Image::getStringSize(t.substr(this->_cursorPos), zoom);

		if (this->_scrollX - toEnd > 0)
			this->_scrollX = toEnd;
		else if (this->_scrollX - toEnd < -d.w)
			this->_scrollX = toEnd - d.w;
		this->_scrollX = std::clamp(this->_scrollX, 0, maxScroll);

		align   = Text::HAlign::Right;
		anchorX = d.x + d.w + this->_scrollX;
		cursorX = d.x + d.w + this->_scrollX - toEnd;
	}

	this->_input.renderClip(anchorX, d.y + d.h / 2, img, d,
							align, Text::VAlign::Center, col);

	if (this->_focused)
	{
		cursorX = std::clamp(cursorX, d.x, d.x + d.w);
		img.putVLine(cursorX, d.y + 2, d.y + d.h - 2, 0xffffff, 1);
	}
}