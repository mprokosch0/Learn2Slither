#include "learn2Slither.hpp"

Image::Image():_mlx(NULL), _img(NULL),
	_addr(NULL), _bitsPerPixel(0), _lineLength(0), _endian(0)
{}

Image::Image(Image const &rhs)
{
	*this = rhs;
}

Image::~Image()
{}

Image	&Image::operator=(Image const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_mlx = rhs._mlx;
	this->_img = rhs._img;
	this->_addr = rhs._addr;
	this->_bitsPerPixel = rhs._bitsPerPixel;
	this->_lineLength = rhs._lineLength;
	this->_endian = rhs._endian;
	return *this;
}

void	*Image::getImg(void) const
{
	return this->_img;
}

char	*Image::getAddr(void) const
{
	return this->_addr;
}

int		Image::getBitsPerPixel(void) const
{
	return this->_bitsPerPixel;
}

int		Image::getLineLength(void) const
{
	return this->_lineLength;
}

int		Image::getEndian(void) const
{
	return this->_endian;
}

void	Image::init(void *mlx, size_t width, size_t height)
{
	if (this->_img)
		return ;
	this->_mlx = mlx;
	this->_width = width;
	this->_height = height;
	if (!this->_mlx)
		throw std::runtime_error("Mlx pointer is NULL.");
	this->_img = mlx_new_image(this->_mlx, width, height);
	if (!this->_img)
		throw std::runtime_error("Trying to initialise an image with NULL.");
	this->_addr = mlx_get_data_addr(this->_img, &this->_bitsPerPixel, &this->_lineLength, &this->_endian);
	if (!this->_addr)
	{
		mlx_destroy_image(this->_mlx, this->_img);
		throw std::runtime_error("Function mlx_get_data_addr failed.");
	}
}

void	Image::destroy(void)
{
	if (this->_img)
		mlx_destroy_image(this->_mlx, this->_img);
}

void	Image::putPixel(int x, int y, int color)
{
	char	*dst;
	if (x < 0 || x >= static_cast<int>(this->_width)
		|| y < 0 || y >= static_cast<int>(this->_height))
		return ;
	dst = this->_addr + (y * this->_lineLength + x * (this->_bitsPerPixel / 8));
	*(unsigned int*)dst = color;
}

void	Image::putRectangle(box &rect, int color)
{
	char	*dst;
	if (rect.x < 0)
	{
		rect.w += rect.x;
		rect.x = 0;
	}
	if (rect.y < 0)
	{
		rect.h += rect.y;
		rect.y = 0;
	}
	if (rect.x + rect.w > static_cast<int>(this->_width))
		rect.w = this->_width - rect.x;
	if (rect.y + rect.h > static_cast<int>(this->_height))
		rect.h = this->_height - rect.y;

	int xRatio = rect.x * (this->_bitsPerPixel / 8);
	for (int i = rect.y; i < rect.y + rect.h; i++)
	{
		dst = this->_addr + (i * this->_lineLength + xRatio);
		std::fill_n(reinterpret_cast<unsigned int*>(dst), rect.w, color);
	}
	
}