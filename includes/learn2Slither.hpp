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
# include <functional>
# include <filesystem>

class Image;
class Engine;

struct box
{
	int	x;
	int	y;
	int	w;
	int	h;
};

class Text
{
	private:
		static size_t	_height;

	private:
		std::string		_text;
		float			_zoom;
		size_t			_width;
		int				_color;
		int				_hoverColor;
	
	public:
		Text(void);
		Text(std::string text, float zoom, int color, int secondCol);
		Text(Text const &rhs);
		~Text();

	public:
		Text	&operator=(Text const &rhs);
	
	public:
		enum class HAlign {Left, Center, Right};
		enum class VAlign {Top, Center, Bottom};
		enum class Colors {Primary, Secondary};
		void	render(int x, int y, Image &img,
			HAlign hAlign = HAlign::Left, VAlign vAlign = VAlign::Center,
			Colors color = Colors::Primary) const;
};

class Rectangle
{
	private:
		box		_dimentions;
		int		_color;
		size_t	_borderWidth;
		int		_borderColor;

	public:
		Rectangle(void);
		Rectangle(box dims, int color, size_t borderWidth, int borderColor);
		Rectangle(Rectangle const &rhs);
		~Rectangle();
	
	public:
		Rectangle &operator=(Rectangle const &rhs);
	
	public:
		void	render(Image &img) const;
		box		getDimentions(void) const;
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
		static int	getStringSize(std::string str, float zoom);
		void		init(void *mlx, size_t width, size_t height);
		void		destroy(void);
		void		colorImage(int color);
		void		putPixel(int x, int y, int color);
		void		putRectangle(box rect, int color);
		void		putRectangleBorder(box rect, int color, size_t borderWidth, int borderColor);
		void		putHLine(int y, int xStart, int xEnd, int color, size_t width);
		void		putVLine(int x, int yStart, int yEnd, int color, size_t width);
		void		putString(int x, int y, std::string text, int color, float zoom);

};

class Button
{
	private:
		static int						_borderWidth;
		Text							_text;
		box								_dimentions;
		int								_color;
		int								_hoverColor;
		int								_borderColor;
		int								_borderHoverColor;
		bool							_hover;
		std::function<void(Engine &)>	_onClickFunction;
	
	public:
		Button(box dim, int color, int hoverColor, int borderColor,
			int borderHovercolor, Text text, std::function<void(Engine &)> func);
		Button(Button const &rhs);
		~Button();
	
	public:
		Button	&operator=(Button const &rhs);
	
	public:
		void	click(Engine &engine) const;
		void	render(Image &img) const;
		// void	renderClip(Image &img, box &clip) const;
		void	clearHover(void);
		bool	hoverIfOnButton(int x, int y);
		bool	isMouseOn(void) const;
		box		getDimentions(void) const;
};

struct Hooks
{
	bool	mouseLeft = false;
	bool	mouseRight = false;
	bool	mouseScrollUp = false;
	bool	mouseScrollDown = false;
	bool	mouseScrollButton = false;
};

class IScreen
{
	protected:
		size_t				_width;
		size_t				_height;
		std::vector<Button>	_buttons;
		Hooks				_hooks;
		enum {HOOK_DOWN, HOOK_UP};
	protected:
		virtual void	updateHooks(int button, bool DownUp);


	public:
		IScreen(size_t width, size_t height):
			_width(width), _height(height){};
		virtual ~IScreen() = default;
		virtual void onEnter() {};
		virtual void onExit() {};
		virtual void update() = 0;
		virtual void render(Image &img) = 0;
		virtual void onKey(int keycode) = 0;
		virtual void onMouseDown(int button, int x, int y, Engine &engine) = 0;
		virtual void onMouseUp(int button, int x, int y, Engine &engine) = 0;
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
		void	onMouseDown(int button, int x, int y, Engine &engine) override;
		void	onMouseUp(int button, int x, int y,Engine &engine) override;
		void	onMouseMove(int x, int y) override;
};

class SelectScreen : public IScreen
{
	private:
		std::vector<Button>									_files;
		std::pair<Text, std::array<int, 2>>					_title;
		std::vector<std::pair<Text, std::array<int, 2>>>	_texts;
		std::vector<Rectangle>								_rectangles;
		Rectangle											_fileList;

	private:
		void	renderScrollList(Image &img) const;


	public:
		SelectScreen(size_t width, size_t height);

	public:
		void	update() override;
		void	render(Image &img) override;
		void	onKey(int keycode) override;
		void	onMouseDown(int button, int x, int y, Engine &engine) override;
		void	onMouseUp(int button, int x, int y,Engine &engine) override;
		void	onMouseMove(int x, int y) override;
};

enum mouseHooks
{
	M_LEFT_CLICK = 1,
	M_SCROLL_BUTTON,
	M_RIGHT_CLICK,
	M_SCROLL_UP,
	M_SCROLL_DOWN
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
		std::unique_ptr<IScreen>	_nextScreen;
		std::exception_ptr			_error;

	private:
		static int	loopTrampoline(void *self);
		static int	mouseDownTrampoline(int button, int x, int y, void *self);
		static int	mouseUpTrampoline(int button, int x, int y, void *self);
		static int	mouseMoveTrampoline(int x, int y, void *self);
		void		mouseDown(int button, int x, int y);
		void		mouseUp(int button, int x, int y);
		void		mouseMove(int x, int y);
		void		loop();


	public:
		Engine(size_t winWidth, size_t winHeight);
		~Engine();
	
	public:
		void	prepareNextScreen(std::unique_ptr<IScreen> next);
		void	run(void);
		void	quit(void);
};

#endif