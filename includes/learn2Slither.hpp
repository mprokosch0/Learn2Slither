#ifndef LEARN2SLITHER_HPP

# define LEARN2SLITHER_HPP

extern "C"
{
	# include "../minilibx-linux/mlx.h"
}

# include "Agent.hpp"
# include <iostream>
# include <string>
# include <memory>
# include <algorithm>
# include <vector>
# include <exception>
# include <unordered_map>
# include <functional>
# include <filesystem>
# include <chrono>
# include <format>

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
		const std::string	&getText(void) const;
		void				setText(std::string text);
		float				getZoom(void) const;
		void				addChar(char c, size_t pos);
		void				deleteChar(size_t pos);
		size_t				pxSize(void) const;
		size_t				size(void) const;
		void				render(int x, int y, Image &img,
								HAlign hAlign = HAlign::Left, VAlign vAlign = VAlign::Center,
								Colors color = Colors::Primary) const;
		void				renderClip(int x, int y, Image &img, box const &clip,
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
		void		render(Image &img) const;
		void		render(Image &img, int color, int borderColor) const;
		const box	getDimentions(void) const;
		int			getBorderWidth(void) const;
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
		void		*getImg(void) const;
		char		*getAddr(void) const;
		int			getBitsPerPixel(void) const;
		int			getLineLength(void) const;
		int			getEndian(void) const;
		static int	getStringSize(std::string str, float zoom);
		static bool	isCharInFont(char c);
	
	public:
		void		init(void *mlx, size_t width, size_t height);
		void		destroy(void);
		void		colorImage(int color);
		void		putPixel(int x, int y, int color);
		void		putRectangle(box rect, int color);
		void		putRectangleBorder(box rect, int color, size_t borderWidth, int borderColor);
		void		putHLine(int y, int xStart, int xEnd, int color, size_t width);
		void		putVLine(int x, int yStart, int yEnd, int color, size_t width);
		void		putString(int x, int y, std::string text, int color, float zoom);
		void		putStringClip(int x, int y, std::string text, box const &clip, int color, float zoom);

};

class Button
{
	private:
		static int									_borderWidth;
		Text										_text;
		int											_textX;
		bool										_ascend;
		std::chrono::_V2::steady_clock::time_point	_tPoint;
		int											_renderCount;
		box											_dimentions;
		int											_color;
		int											_hoverColor;
		int											_borderColor;
		int											_borderHoverColor;
		int											_pressedColor;
		int											_pressedBorderColor;
		bool										_hover;
		bool										_active;
		bool										_pressed;
		std::function<void()>						_onClickFunction;
	
	public:
		Button(void);
		Button(box dim, int color, int hoverColor, int borderColor,
			int borderHovercolor, int pressedColor, int pressedBorderColor, Text text, bool active,
			std::function<void()> func);
		~Button();
	
	public:
		void		click(void);
		bool		isPressed(void) const;
		void		setPressed(bool press);
		void		render(Image &img);
		void		renderClip(Image &img, box &clip);
		void		clearHover(void);
		bool		hoverIfOnButton(int x, int y);
		bool		hoverIfOnButtonClip(int x, int y, box &clip);
		bool		isMouseOn(void) const;
		void		setDimentions(box &dim);
		const box	&getDimentions(void) const;
		const Text	&getText(void) const;
		void		setText(std::string txt);
		bool		isActive(void) const;
		void		activate(void);
		void		desactivate(void);

};

class TextBox
{
	private:
		Rectangle									_box;
		Text										_input;
		size_t										_cursorPos;
		int											_inputX;
		bool										_focused;
		bool										_active;
		int											_inactiveColor;
		int											_inactiveColorBorder;
		std::chrono::_V2::steady_clock::time_point	_tPoint;
	
	
	public:
		TextBox(void);
		TextBox(Text txt, Rectangle box, int inactiveColor, int inactiveColorBorder);
		~TextBox();
	
	public:
		bool		isFocused(void) const;
		bool		isMouseOn(int x, int y) const;
		void		activate(void);
		void		desactivate(void);
		std::string	getText(void) const;
		void		setText(std::string txt);
		void		doAction(int keycode);
		void		setfocus(bool focus);
		void		render(Image &img);
};

template<typename T>
class Slider
{
	private:
		box		_dims;
		Button	_slider;
		int		_trackColor;
		int		_trackColorDesac;
		T		_vMin;
		T		_vMax;
		bool	_active;
	
	
	public:
		Slider(void);
		Slider(box dim, int trackColor, int trackColorDesac, int color,
			int hoverColor, int borderColor,
			int borderHovercolor, int pressedColor, int pressedBorderColor,
			T valueMin, T valueMax, bool active = true);
		Slider(const Slider &rhs) = default;
		~Slider();
	
	public:
		void	click(int x, T &value);
		bool	isMouseOn(void) const;
		void	clearHover(void);
		void	updatePos(T const &value);
		bool	hoverIfOnSlider(int x, int y);
		void	activate(void);
		void	desactivate(void);
		void	render(Image &img);
};

# include "Slider.tpp"

struct Hooks
{
	bool	mouseLeft = false;
	bool	mouseRight = false;
	bool	mouseScrollUp = false;
	bool	mouseScrollDown = false;
	bool	mouseScrollButton = false;
	bool	lShiftKey = false;
};

enum class keycodes
{
	BACKSPACE = 65288,
	DEL = 65535,
	HOME = 65360,
	L_SHIFT = 65505,
	R_SHIFT = 65506,
	ARR_LEFT = 65361,
	ARR_UP = 65362,
	ARR_RIGHT = 65363,
	ARR_DOWN = 65364,
	END = 65367,
	ZERO = 65438,
	ONE = 65436,
	TWO = 65433,
	THREE = 65435,
	FOUR = 65430,
	FIVE = 65437,
	SIX = 65432,
	SEVEN = 65429,
	EIGHT = 65431,
	NINE = 65434
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
		virtual void	updateMouseHooks(int button, bool DownUp);
		virtual void	updateKeyHooks(keycodes keycode, bool DownUp);


	public:
		IScreen(size_t width, size_t height):
			_width(width), _height(height){};
		virtual ~IScreen() = default;
		virtual void onEnter() {};
		virtual void onExit() {};
		virtual void update() = 0;
		virtual void render(Image &img) = 0;
		virtual void onKeyDown(int keycode) = 0;
		virtual void onKeyUp(int keycode) = 0;
		virtual void onMouseDown(int button, int x, int y) = 0;
		virtual void onMouseUp(int button, int x, int y) = 0;
		virtual void onMouseMove(int x, int y) = 0;
};

class TitleScreen : public IScreen
{
	public:
		TitleScreen(size_t width, size_t height, Engine &engine);

	public:
		void	update() override;
		void	render(Image &img) override;
		void	onKeyDown(int keycode) override;
		void	onKeyUp(int keycode) override;
		void	onMouseDown(int button, int x, int y) override;
		void	onMouseUp(int button, int x, int y) override;
		void	onMouseMove(int x, int y) override;
};

class SelectScreen : public IScreen
{
	private:
		using textCo = std::vector<std::pair<Text, std::array<int, 2>>>;
		struct selectedFile
		{
			std::shared_ptr<Agent>	file;
			textCo					titles;
			textCo					names;
			textCo					values;
			std::vector<Rectangle>	rectangles;
			Button					useModel;
		};
		struct settings
		{
			std::shared_ptr<Agent>			model;
			bool							useModel;
			bool							exportModel;
			TextBox							exportInput;
			size_t							width;
			Slider<size_t>					wSlider;
			size_t							height;
			Slider<size_t>					hSlider;
			textCo							titles;
			textCo							names;
			textCo							values;
			std::vector<Rectangle>			rectangles;
			std::array<Button, 3>			boardSize;
			std::array<Button, 2>			onOffImport;
			std::array<Button, 2>			onOffExport;
			std::array<Button, 2>			onOffLearning;
			bool							learning;
			std::array<Button, 2>			onOffDisplay;
			bool							visualDisplay;
			std::array<double, 3>			hPVs;
			textCo							hyperParametersValues;
			std::array<Slider<double>, 3>	hyperParametersSlider;
			std::vector<Button>				buttons;
		};

	private:
		std::vector<Button>					_files;
		std::pair<Text, std::array<int, 2>>	_title;
		textCo								_texts;
		std::vector<Rectangle>				_rectangles;
		Rectangle							_fileListRect;
		selectedFile						_selectedFile;
		settings							_settings;

	private:
		void	initModelSelection(void);
		void	initSettings(void);
		void	addEntry(void);
		void	removeEntry(void);
		void	updateFilesVector(void);
		void	hoverUpdate(int x, int y);
		void	scrollButtonsFiles(int button);
		void	renderScrollList(Image &img);
		void	renderSettings(Image &img);
		void	renderPreview(Image &img) const;
		void	renderSelectedFile(Image &img);


	public:
		SelectScreen(size_t width, size_t height, Engine &engine);

	public:
		void	update() override;
		void	render(Image &img) override;
		void	onKeyDown(int keycode) override;
		void	onKeyUp(int keycode) override;
		void	onMouseDown(int button, int x, int y) override;
		void	onMouseUp(int button, int x, int y) override;
		void	onMouseMove(int x, int y) override;
};

class GameScreen : public IScreen
{
	private:
		Board		_board;
		Agent		_agent;
		bool		_visualDisplay;
		std::string	_exportName;
	

	public:
		GameScreen(size_t width, size_t height, Engine &engine,
			Board board, Agent agent, bool visualDisplay, std::string exportName);
	
	public:
		void	update() override;
		void	render(Image &img) override;
		void	onKeyDown(int keycode) override;
		void	onKeyUp(int keycode) override;
		void	onMouseDown(int button, int x, int y) override;
		void	onMouseUp(int button, int x, int y) override;
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
		static int	onKeyDownTrampoline(int keycode, void *self);
		static int	onKeyUpTrampoline(int keycode, void *self);
		void		mouseDown(int button, int x, int y);
		void		mouseUp(int button, int x, int y);
		void		mouseMove(int x, int y);
		void		onKeyDown(int keycode);
		void		onKeyUp(int keycode);
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