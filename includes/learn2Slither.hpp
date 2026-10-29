#ifndef LEARN2SLITHER_HPP

# define LEARN2SLITHER_HPP

extern "C"
{
	# include "../minilibx-linux/mlx.h"
}

# include <iostream>
# include <string>
# include <memory>
# include <algorithm>

struct box
{
	int	x;
	int	y;
	int	w;
	int	h;
};

class Image
{
	private:
		void	*_mlx;
		void	*_img;
		char	*_addr;
		int		_bitsPerPixel;
		int		_lineLength;
		int		_endian;
		size_t	_width;
		size_t	_height;
	
	
	public:
		Image(void);
		Image(Image const &rhs);
		~Image();
	
	public:
		Image	&operator=(Image const &rhs);
	
	public:
		void	*getImg(void) const;
		char	*getAddr(void) const;
		int		getBitsPerPixel(void) const;
		int		getLineLength(void) const;
		int		getEndian(void) const;
	
	public:
		void	init(void *mlx, size_t width, size_t height);
		void	destroy(void);
		void	putPixel(int x, int y, int color);
		void	putRectangle(box &rect, int color);

};

class Button
{
	private:
		static int	_borderWidth;
		box			_dimentions;
		int			_color;
		bool		_hover;
	
	public:
		Button(box dim, int color);
		Button(Button const &rhs);
		~Button();
	
	public:
		Button	&operator=(Button const &rhs);
	
	public:
		void	render(Image &img);
};

class IScreen
{
	protected:
		size_t	_width;
		size_t	_height;

	public:
		IScreen(size_t width, size_t height):
			_width(width), _height(height){};
		virtual ~IScreen() = default;
		virtual void onEnter() {};
		virtual void onExit() {};
		virtual void update() = 0;
		virtual void render(Image &img) = 0;
		virtual void onKey(int keycode) = 0;
		virtual void onMouse(/*int button, int x, int y*/){};
};

class TitleScreen : public IScreen
{
	private:


	public:
		TitleScreen(size_t width, size_t height);

	public:
		void update() override;
		void render(Image &img) override;
		void onKey(int keycode) override;
};

class Engine
{
	private:
		void						*_mlx;
		void						*_mlx_win;
		size_t						_width;
		size_t						_height;
		Image						_img;
		std::unique_ptr<IScreen>	_screen;

	private:
		static int	loopTrampoline(void *self);
		void		loop();


	public:
		Engine(size_t winWidth, size_t winHeight);
		~Engine();
	
	public:
		void	run(void);
};

#endif