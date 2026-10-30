#include "learn2Slither.hpp"

std::unordered_map<char, std::vector<std::vector<int>>>	Image::_font;

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

typedef struct s_img
{
	void	*img;
	char	*addr = nullptr;
	int		bpp = 0;
	int		line_l = 0;
	int		endian = 0;
}	t_img;

static int	getPixel(t_img &img, int x, int y)
{
	char *dst;
	dst = img.addr + (y * img.line_l + x * (img.bpp / 8));
	if (dst)
		return *(unsigned int *)dst;
	return 0;
}

void	Image::importFont(void)
{
	const std::string set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-+,;:!?.*&#%");
	std::string rep = "font/", ext = ".xpm";
	int width = 0, height = 0; 
	for (char c : set)
	{
		std::string filename = rep + c + ext;
		t_img img;
		img.img = mlx_xpm_file_to_image(this->_mlx, filename.data(), &width, &height);
		if (!img.img)
			throw std::runtime_error("Function mlx_xpm_file_to_img failed on " + filename + " file.");
		img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_l, &img.endian);
		std::vector<std::vector<int>> pixels;
		pixels.reserve(width);
		for (int i = 0; i < width; i++)
		{
			std::vector<int> line;
			line.reserve(height);
			for (int j = 0; j < height; j++)
			{
				int pixel = getPixel(img, i, j);
				if (pixel != 0xffffff)
					pixel = 0x101010;
				line.emplace_back(pixel);
			}
			pixels.emplace_back(line);
		}
		this->_font.emplace(c, pixels);
		mlx_destroy_image(this->_mlx, img.img);
	}
	this->_font.emplace(' ', std::vector<std::vector<int>>({{0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff}}));
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
	if (!_font.size())
		importFont();
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

int		Image::getStringSize(std::string &str)
{
	int size = 0;
	for (char c : str)
	{
		if (Image::_font.find(c) == Image::_font.end())
			throw std::runtime_error("Unrecognise char in text. Please refers you to the font");
		size += Image::_font[c].size();
	}
	return size;
}

void	Image::putString(int x, int y, std::string text)
{
	for (char c: text)
	{
		if (this->_font.find(c) == this->_font.end())
			throw std::runtime_error("Unrecognise char in text. Please refers you to the font");
		std::vector<std::vector<int>> &pixels = this->_font[c];
		for (size_t i = 0; i < pixels.size(); i++)
			for (size_t j = 0; j < pixels[i].size(); j++)
				if (pixels[i][j] != 0xffffff)
					this->putPixel(x + i, y + j, pixels[i][j]);
		x += pixels.size();
	}
}