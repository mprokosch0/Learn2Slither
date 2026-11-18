# include "learn2Slither.hpp"

Texture::Texture(void): _mlx(NULL), _img(NULL), _addr(NULL), _bitsPerPixel(0), _lineLength(0), _endian(0),
	_width(0), _height(0), _blockSize()
{}

Texture::Texture(void *mlx, std::string path, std::pair<int, int> blocSize):
	_mlx(mlx), _img(NULL), _addr(NULL), _bitsPerPixel(0), _lineLength(0), _endian(0),
	_width(0), _height(0), _blockSize(blocSize)
{
	this->_img = mlx_xpm_file_to_image(mlx, path.data(), &this->_width, &this->_height);
	if (!this->_img)
		throw std::runtime_error("Function mlx_xpm_file_to_img failed on " + path + " file.");
	this->_addr = mlx_get_data_addr(this->_img, &this->_bitsPerPixel, &this->_lineLength, &this->_endian);
	if (!this->_addr)
	{
		mlx_destroy_image(this->_mlx, this->_img);
		throw std::runtime_error("Function mlx_get_data_addr failed on " + path + " file.");
	}
}


Texture::~Texture(void)
{
	if (this->_mlx && this->_img)
		mlx_destroy_image(this->_mlx, this->_img);
}

void	*Texture::getImg(void) const
{
	return this->_img;
}

void	Texture::applyBloc(Image &img, std::pair<int, int> bloc, int x, int y) const
{
	if (!this->_mlx || !this->_img)
		return ;
	if (bloc.first < 0 || bloc.first >= this->_width / this->_blockSize.first
		|| bloc.second < 0 || bloc.second >= this->_height / this->_blockSize.second)
		return ;
	int xb = bloc.first * this->_blockSize.first;
	int yb = bloc.second * this->_blockSize.second;
	char *dst;
	for (int i = 0; i < this->_blockSize.second; i++)
	{
		for (int j = xb; j < xb + this->_blockSize.first; j++)
		{
			dst = this->_addr + ((yb + i) * this->_lineLength + j * (this->_bitsPerPixel / 8));
			img.putPixel(x + (j - xb), y + i, *(unsigned int *)dst);
		}
	}
}

void	Texture::applyAll(Image &img, int x, int y) const
{
	if (!this->_mlx || !this->_img)
		return ;
	char *dst;
	for (int i = 0; i < this->_height; i++)
	{
		for (int j = 0; j < this->_width; j++)
		{
			dst = this->_addr + (i * this->_lineLength + j * (this->_bitsPerPixel / 8));
			img.putPixel(x + j, y + i, *(unsigned int *)dst);
		}
	}
}