#include "learn2Slither.hpp"

template<typename T>
Slider<T>::Slider(void): _dims((box){0, 0, 0, 0}), _slider(), _trackColor(0),
	_trackColorDesac(0), _vMin(), _vMax(), _active(true)
{}

template<typename T>
Slider<T>::Slider(box dim, int trackColor, int trackColorDesac,
		int color, int hovercolor, int bordercolor,
		int borderHoverColor, int pressedColor, int pressedBorderColor,
		T valueMin, T valueMax, bool active): _dims(dim),
		_trackColor(trackColor), _trackColorDesac(trackColorDesac),
		_vMin(valueMin), _vMax(valueMax), _active(active)
{
	box bDIms = {dim.x - 4, dim.y + 1 - dim.h - 7, 8, (dim.h * 2) + 1 + 14};
	this->_slider = Button(bDIms, color, hovercolor, bordercolor,
		borderHoverColor, pressedColor, pressedBorderColor, Text(), true, [](){});
}

template<typename T>
Slider<T>::~Slider(void)
{}

template<typename T>
bool	Slider<T>::isMouseOn(void) const
{
	if (!this->_active)
		return false;
	return this->_slider.isMouseOn();
}

template<typename T>
void	Slider<T>::clearHover(void)
{
	if (!this->_active)
		return ;
	this->_slider.clearHover();
}

template<typename T>
void	Slider<T>::updatePos(T const &value)
{
	if (!this->_active)
		return ;
	const int half = 4;
	float ratio = static_cast<float>(value - this->_vMin) / (this->_vMax - this->_vMin);
	box dim = this->_slider.getDimentions();
	dim.x = this->_dims.x - half + ratio * this->_dims.w;
	this->_slider.setDimentions(dim);
}

template<typename T>
bool	Slider<T>::hoverIfOnSlider(int x, int y)
{
	if (!this->_active)
		return false;
	return this->_slider.hoverIfOnButton(x, y);
}

template<typename T>
void	Slider<T>::activate(void)
{
	this->_active = true;
}

template<typename T>
void	Slider<T>::desactivate(void)
{
	this->_active = false;
}

template<typename T>
void	Slider<T>::click(int x, T &value)
{
	if (!this->_active)
		return ;
	const int half = 4;
	int minX = this->_dims.x;
	int maxX = this->_dims.x + this->_dims.w;

	if (x < minX)
		x = minX;
	if (x > maxX)
		x = maxX;

	box dim = this->_slider.getDimentions();
	dim.x = x - half;
	this->_slider.setDimentions(dim);

	float ratio = static_cast<float>(x - minX) / this->_dims.w;
	value = static_cast<T>(this->_vMin + ratio * (this->_vMax - this->_vMin));
}

template<typename T>
void	Slider<T>::render(Image &img)
{
	int tColor = (this->_active) ? this->_trackColor : this->_trackColorDesac;
	img.putRectangle(this->_dims, tColor);
	if (!this->_active)
		this->_slider.setPressed(true);
	this->_slider.render(img);
	if (!this->_active)
		this->_slider.setPressed(false);
}