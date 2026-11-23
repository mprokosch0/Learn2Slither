#include "learn2Slither.hpp"

Rectangle::Rectangle(void): _dimentions((box){0, 0, 0, 0}), _color(0xffffff),
	_borderWidth(0), _borderColor(0xffffff)
{}

Rectangle::Rectangle(box dims, int color, size_t borderWidth, int borderColor):
	_dimentions(dims), _color(color), _borderWidth(borderWidth),
	_borderColor(borderColor)
{}

Rectangle::Rectangle(Rectangle const &rhs): _dimentions(rhs._dimentions),
	_color(rhs._color), _borderWidth(rhs._borderWidth),
	_borderColor(rhs._borderColor)
{}

Rectangle::~Rectangle()
{}

Rectangle	&Rectangle::operator=(Rectangle const &rhs)
{
	if (this == &rhs)
		return *this;

	this->_dimentions = rhs._dimentions;
	this->_color = rhs._color;
	this->_borderWidth = rhs._borderWidth;
	this->_borderColor = rhs._borderColor;
	return *this;
}

void	Rectangle::setColors(int color, int borderColor)
{
	this->_color = color;
	this->_borderColor = borderColor;
}

const box	Rectangle::getDimentions(void) const
{
	return this->_dimentions;
}

int		Rectangle::getBorderWidth(void) const
{
	return this->_borderWidth;
}

void	Rectangle::render(Image &img) const
{
	img.putRectangleBorder(this->_dimentions, this->_color,
		this->_borderWidth, this->_borderColor);
}

void	Rectangle::render(Image &img, int color, int borderColor) const
{
	img.putRectangleBorder(this->_dimentions, color,
		this->_borderWidth, borderColor);
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

CornerLessRect::CornerLessRect(void)
{}

CornerLessRect::CornerLessRect(box dims, int color, size_t borderWidth, int borderColor,
	uint8_t whichCorner, box cornerDims):
	Rectangle(dims, color, borderWidth, borderColor), _whichCorner(whichCorner), _cornerDims(cornerDims)
{}

void	CornerLessRect::render(Image &img) const
{
	if (!(this->_whichCorner & 15))
	{
		render(img);
		return ;
	}
	box rectTop = _dimentions;
	rectTop.h = _cornerDims.h;
	if (this->_whichCorner & static_cast<uint8_t>(Corners::TOP_LEFT))
	{
		rectTop.x += _cornerDims.w;
		rectTop.w -= _cornerDims.w;
	}
	if (this->_whichCorner & static_cast<uint8_t>(Corners::TOP_RIGHT))
		rectTop.w -= _cornerDims.w;
	img.putRectangle(rectTop, this->_color);

	box rectTopBord = rectTop;
	rectTopBord.h = this->_borderWidth;
	img.putRectangle(rectTopBord, this->_borderColor);
	rectTopBord.h = rectTop.h;
	rectTopBord.w = this->_borderWidth;
	img.putRectangle(rectTopBord, this->_borderColor);
	rectTopBord.x += rectTop.w - this->_borderWidth;
	img.putRectangle(rectTopBord, this->_borderColor);

	box rectDown = _dimentions;
	rectDown.y = rectDown.y + rectDown.h - _cornerDims.h;
	rectDown.h = _cornerDims.h;
	if (this->_whichCorner & static_cast<uint8_t>(Corners::BOTTOM_LEFT))
	{
		rectDown.x += _cornerDims.w;
		rectDown.w -= _cornerDims.w;
	}
	if (this->_whichCorner & static_cast<uint8_t>(Corners::BOTTOM_RIGHT))
		rectDown.w -= _cornerDims.w;
	img.putRectangle(rectDown, this->_color);

	box rectDownBord = rectDown;
	rectDownBord.y += rectDown.h - this->_borderWidth;
	rectDownBord.h = this->_borderWidth;
	img.putRectangle(rectDownBord, this->_borderColor);
	rectDownBord = rectDown;
	rectDownBord.w = this->_borderWidth;
	img.putRectangle(rectDownBord, this->_borderColor);
	rectDownBord.x += rectDown.w - this->_borderWidth;
	img.putRectangle(rectDownBord, this->_borderColor);

	box rectMid = _dimentions;
	rectMid.y += _cornerDims.h;
	rectMid.h -= _cornerDims.h * 2;
	img.putRectangle(rectMid, this->_color);
	rectMid.w = 2;
	img.putRectangle(rectMid, this->_borderColor);
	rectMid.x += _dimentions.w - this->_borderWidth;
	img.putRectangle(rectMid, this->_borderColor);

	if (this->_whichCorner & static_cast<uint8_t>(Corners::TOP_LEFT))
	{
		rectMid = {_dimentions.x,
					_dimentions.y + _cornerDims.h,
					_cornerDims.w + (int)this->_borderWidth, 2};
		img.putRectangle(rectMid, this->_borderColor);
	}
	if (this->_whichCorner & static_cast<uint8_t>(Corners::TOP_RIGHT))
	{
		rectMid = {_dimentions.x + _dimentions.w - _cornerDims.w - (int)this->_borderWidth,
					_dimentions.y + _cornerDims.h,
					_cornerDims.w, 2};
		img.putRectangle(rectMid, this->_borderColor);
	}
	if (this->_whichCorner & static_cast<uint8_t>(Corners::BOTTOM_LEFT))
	{
		rectMid = {_dimentions.x,
					_dimentions.y + _dimentions.h - _cornerDims.h - (int)this->_borderWidth,
					_cornerDims.w + (int)this->_borderWidth, 2};
		img.putRectangle(rectMid, this->_borderColor);
	}
	if (this->_whichCorner & static_cast<uint8_t>(Corners::BOTTOM_RIGHT))
	{
		rectMid = {_dimentions.x + _dimentions.w - _cornerDims.w - (int)this->_borderWidth,
					_dimentions.y + _dimentions.h - _cornerDims.h - (int)this->_borderWidth,
					_cornerDims.w, 2};
		img.putRectangle(rectMid, this->_borderColor);
	}
}