#include "learn2Slither.hpp"

template<typename T>
Slider<T>::Slider(void): _dims((box){0, 0, 0, 0}), _slider(), _vMin(), _vMax()
{}

template<typename T>
Slider<T>::Slider(box dim, int color, int hovercolor, int bordercolor,
		int borderHoverColor, T valueMin, T valueMax): _dims(dim),
		_vMin(valueMin), _vMax(valueMax)
{
	box bDIms = {dim.x - 4, dim.y + 1 - dim.h - 7, 8, (dim.h * 2) + 1 + 14};
	this->_slider = Button(bDIms, color, hovercolor, bordercolor,
		borderHoverColor, color, hovercolor, Text(), true, [](){});
}

template<typename T>
Slider<T>::~Slider(void)
{}

template<typename T>
bool	Slider<T>::isMouseOn(void) const
{
	return this->_slider.isMouseOn();
}

template<typename T>
void	Slider<T>::clearHover(void)
{
	this->_slider.clearHover();
}

template<typename T>
void	Slider<T>::updatePos(T const &value)
{
	const int half = 4;
	float ratio = static_cast<float>(value - this->_vMin) / (this->_vMax - this->_vMin);
	box dim = this->_slider.getDimentions();
	dim.x = this->_dims.x - half + ratio * this->_dims.w;
	this->_slider.setDimentions(dim);
}

template<typename T>
bool	Slider<T>::hoverIfOnSlider(int x, int y)
{
	return this->_slider.hoverIfOnButton(x, y);
}

template<typename T>
void Slider<T>::click(int x, T &value)
{
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
void	Slider<T>::render(Image &img) const
{
	img.putRectangle(this->_dims, 0x0);
	this->_slider.render(img);
}