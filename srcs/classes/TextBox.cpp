#include "learn2Slither.hpp"

TextBox::TextBox(void): _box(), _input(), _cursorPos(0), _inputX(),
	_focused(false), _active(false), _inactiveColor(), _inactiveColorBorder(),
	_tPoint(std::chrono::steady_clock::time_point())
{}

TextBox::TextBox(Text txt, Rectangle box, int inactiveColor, int inactiveColorBorder):
	_box(box), _input(txt), _cursorPos(0),
	_inputX(), _focused(false), _active(false),  _inactiveColor(inactiveColor),
	_inactiveColorBorder(inactiveColorBorder), _tPoint(std::chrono::steady_clock::time_point())
{
	this->_inputX = this->_box.getDimentions().x + this->_box.getBorderWidth();
}

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

std::string	TextBox::getText(void) const
{
	if (this->_active)
		return this->_input.getText();
	return "";
}

void	TextBox::setText(std::string txt)
{
	this->_input.setText(txt);
	this->_cursorPos = txt.size();
}

void	TextBox::doAction(int keycode)
{
	if (!this->_active)
		return ;
	if (keycode < 256 && Image::isCharInFont(keycode))
		this->_input.addChar(keycode, this->_cursorPos++);
	else if (keycode == ' ')
		this->_input.addChar(' ', this->_cursorPos++);
	else if (this->_cursorPos > 0 && static_cast<keycodes>(keycode) == keycodes::BACKSPACE)
		this->_input.deleteChar(this->_cursorPos-- - 1);
	else if (this->_cursorPos < this->_input.size() && static_cast<keycodes>(keycode) == keycodes::DEL)
		this->_input.deleteChar(this->_cursorPos + 1);
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

void	TextBox::render(Image &img)
{
	box d = this->_box.getDimentions();
	const std::string t = this->_input.getText();
	int bw = this->_box.getBorderWidth();
	d.x += bw + 2;
	d.y += bw;
	d.w -= bw * 2;
	d.h -= bw * 2;
	if (this->_active)
		this->_box.render(img);
	else
		this->_box.render(img, this->_inactiveColor, this->_inactiveColorBorder);
	Text::Colors col = (this->_active) ? Text::Colors::Primary : Text::Colors::Secondary;
	this->_input.renderClip(d.x, d.y + (d.h / 2), img, d, Text::HAlign::Left, Text::VAlign::Center, col);
	if (this->_focused)
 		img.putVLine(d.x + Image::getStringSize(std::string(t.begin(), t.begin() + this->_cursorPos), this->_input.getZoom()),
				d.y + 2, d.y + d.h - 2, 0xffffff, 1);
}