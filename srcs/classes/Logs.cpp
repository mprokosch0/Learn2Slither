#include "learn2Slither.hpp"

size_t	Logs::_maxLogNb = 50;

Logs::Logs(void): _dims({0, 0, 0, 0}), _yScroll(0),
	_color(0), _borderColor(0), _infoColor(0),
	_successColor(0), _warnColor(0), _errorColor(0)
{}

Logs::Logs(box dims, int color, int borderColor,
	int infoColor, int successColor, int warnColor, int errorColor):
	_dims(dims), _yScroll(dims.y + dims.h), _color(color),
	_borderColor(borderColor), _infoColor(infoColor),
	_successColor(successColor), _warnColor(warnColor), _errorColor(errorColor)
{}

Logs::~Logs()
{}

bool	Logs::isMouseOn(int x, int y)
{
	if (x < this->_dims.x || x >= this->_dims.x + this->_dims.w
		|| y < this->_dims.y || y >= this->_dims.y + this->_dims.h)
		return false;
	return true;
}

void	Logs::setColors(int color, int borderColor,
	int infoColor, int successColor, int warnColor, int errorColor)
{
	this->_color = color;
	this->_borderColor = borderColor;
	this->_infoColor = infoColor;
	this->_successColor = successColor;
	this->_warnColor = warnColor;
	this->_errorColor = errorColor;
	int col = 0;
	for (auto &t : this->_lines)
	{
		switch (t.second)
		{
			case Colors::INFO:
				col = this->_infoColor;
				break;
			
			case Colors::SUCCESS:
				col = this->_successColor;
				break;
			
			case Colors::WARN:
				col = this->_warnColor;
				break;
			
			case Colors::ERROR:
				col = this->_errorColor;
				break;

		}
		t.first.setColors(col, col);
	}
}

void	Logs::addLog(std::string str, Colors color)
{
	if (this->_lines.size() > this->_maxLogNb)
		this->_lines.pop_back();
	int col = 0;
	switch (color)
	{
		case Colors::INFO:
			col = this->_infoColor;
			break;
		
		case Colors::SUCCESS:
			col = this->_successColor;
			break;
		
		case Colors::WARN:
			col = this->_warnColor;
			break;
		
		case Colors::ERROR:
			col = this->_errorColor;
			break;

	}
	this->_lines.emplace_front(std::make_pair(Text(str, 1.f, col, col), color));
}	

void	Logs::render(Image &img) const
{
	img.putRectangleBorder(this->_dims, this->_color, 2, this->_borderColor);
	int x = this->_dims.x + 5;
	int y = this->_dims.y + this->_dims.h - 10;
	box clip = this->_dims;
	clip.y += 2;
	clip.w -= 4;
	for (auto &t : this->_lines)
	{
		t.first.renderClip(x, y, img, clip, Text::HAlign::Left, Text::VAlign::Bottom);
		y -= 25;
	}
}