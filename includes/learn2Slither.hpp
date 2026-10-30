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
# include <vector>
# include <exception>
# include <unordered_map>

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
		void															*_mlx;
		void															*_img;
		char															*_addr;
		int																_bitsPerPixel;
		int																_lineLength;
		int																_endian;
		static	std::unordered_map<char, std::vector<std::vector<int>>>	_font;
		size_t															_width;
		size_t															_height;
	
	private:
		void	importFont(void);
	
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
		static int	getStringSize(std::string &str);
		void		init(void *mlx, size_t width, size_t height);
		void		destroy(void);
		void		putPixel(int x, int y, int color);
		void		putRectangle(box &rect, int color);
		void		putString(int x, int y, std::string text);

};

class Button
{
	private:
		static int	_borderWidth;
		std::string	_text;
		int			_textSize;
		box			_dimentions;
		int			_color;
		bool		_hover;
	
	public:
		Button(box dim, int color, std::string text);
		Button(Button const &rhs);
		~Button();
	
	public:
		Button	&operator=(Button const &rhs);
	
	public:
		void	render(Image &img);
		void	clearHover(void);
		bool	hoverIfOnButton(int x, int y);
};

class IScreen
{
	protected:
		size_t				_width;
		size_t				_height;
		std::vector<Button>	_buttons;

	public:
		IScreen(size_t width, size_t height):
			_width(width), _height(height){};
		virtual ~IScreen() = default;
		virtual void onEnter() {};
		virtual void onExit() {};
		virtual void update() = 0;
		virtual void render(Image &img) = 0;
		virtual void onKey(int keycode) = 0;
		virtual void onMouseMove(int x, int y) = 0;
};

class TitleScreen : public IScreen
{
	public:
		TitleScreen(size_t width, size_t height);

	public:
		void	update() override;
		void	render(Image &img) override;
		void	onKey(int keycode) override;
		void	onMouseMove(int x, int y) override;
};

enum events
{
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_EXPOSE = 12,
	ON_DESTROY = 17
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
		std::exception_ptr			_error;

	private:
		static int	loopTrampoline(void *self);
		static int	mouseMoveTrampoline(int x, int y, void *self);
		void		mouseMove(int x, int y);
		void		loop();


	public:
		Engine(size_t winWidth, size_t winHeight);
		~Engine();
	
	public:
		void	run(void);
};

#endif