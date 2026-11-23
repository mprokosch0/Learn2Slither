#include "learn2Slither.hpp"

Engine::Engine(size_t winWidth, size_t winHeight): _width(winWidth),
	_height(winHeight), _theme(1), _nextScreen(nullptr), _error(nullptr)
{
	try
	{
		const Theme &t = THEMES[_theme % 3];
		this->_mlx = mlx_init();
		if (!this->_mlx)
			throw std::runtime_error("Mlx_init function failed");
		this->_mlx_win = mlx_new_window(this->_mlx, this->_width, this->_height, std::string("Learn2Slither").data());
		if (!this->_mlx_win)
		{
			mlx_destroy_display(this->_mlx);
			free(this->_mlx);
			throw std::runtime_error("Mlx new window function failed");
		}
		this->_img.init(this->_mlx, winWidth, winHeight);
		this->_screen = std::make_unique<TitleScreen>(this->_width, this->_height, *this);
		int w = this->_width;
		this->_themeRect = Rectangle((box){w - 100, 5, 95, 35},
							t.panel, 2, t.borderBox);
		this->_themesButtons[0] = Button((box){w - 95, 10, 25, 25},
				t.inner, t.hover, t.borderBtn, t.borderHover, t.inner, t.hover,
				Text("", 1.f, t.textMain, t.textSecondary), true,
				[this]()
				{
					this->_theme = 0;
					const Theme &t = THEMES[_theme % 3];
					this->_screen->updateTheme();
					this->_themeRect.setColors(t.panel, t.borderBox);
					for (auto &b : this->_themesButtons)
						b.setColors(t.inner, t.hover, t.borderBtn,
							t.borderHover, t.inner, t.hover, t.textMain, t.textSecondary);
				});
		this->_themesButtons[1] = Button((box){w - 65, 10, 25, 25},
				t.inner, t.hover, t.borderBtn, t.borderHover, t.inner, t.hover,
				Text("", 1.f, t.textMain, t.textSecondary), true,
				[this]()
				{
					this->_theme = 1;
					const Theme &t = THEMES[_theme % 3];
					this->_screen->updateTheme();
					this->_themeRect.setColors(t.panel, t.borderBox);
					for (auto &b : this->_themesButtons)
						b.setColors(t.inner, t.hover, t.borderBtn,
							t.borderHover, t.inner, t.hover, t.textMain, t.textSecondary);
					
				});
		this->_themesButtons[2] = Button((box){w - 35, 10, 25, 25},
				t.inner, t.hover, t.borderBtn, t.borderHover, t.inner, t.hover,
				Text("", 1.f, t.textMain, t.textSecondary), true,
				[this]()
				{
					this->_theme = 2;
					const Theme &t = THEMES[_theme % 3];
					this->_screen->updateTheme();
					this->_themeRect.setColors(t.panel, t.borderBox);
					for (auto &b : this->_themesButtons)
						b.setColors(t.inner, t.hover, t.borderBtn,
							t.borderHover, t.inner, t.hover, t.textMain, t.textSecondary);
				});
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		this->_img.destroy();
		if (this->_mlx_win)
			mlx_destroy_window(this->_mlx, this->_mlx_win);
    	if (this->_mlx)
		{
			mlx_destroy_display(this->_mlx);
			free(this->_mlx);
		}
		throw;
	}
}

Engine::~Engine()
{
	this->_textures.clear();
	this->_img.destroy();
	mlx_destroy_window(this->_mlx, this->_mlx_win);
	mlx_destroy_display(this->_mlx);
	free(this->_mlx);
}

const std::unordered_map<std::string, Texture>	&Engine::getTextures(void) const
{
	return this->_textures;
}

void	Engine::addTexture(std::string path, std::pair<int, int> blockSize)
{
	this->_textures.try_emplace(path, this->_mlx, path, blockSize);
}

int		&Engine::getTheme(void)
{
	return this->_theme;
}

int		Engine::loopTrampoline(void *self)
{
	try
	{
		static_cast<Engine *>(self)->loop();
	}
	catch(const std::exception& e)
	{
		static_cast<Engine *>(self)->_error = std::current_exception();
		mlx_loop_end(static_cast<Engine *>(self)->_mlx);
	}
	return 0;
}

int		Engine::mouseDownTrampoline(int button, int x, int y, void *self)
{
	try
	{
		static_cast<Engine *>(self)->mouseDown(button, x, y);
	}
	catch(const std::exception& e)
	{
		static_cast<Engine *>(self)->_error = std::current_exception();
		mlx_loop_end(static_cast<Engine *>(self)->_mlx);
	}
	return 0;
}

int		Engine::mouseUpTrampoline(int button, int x, int y, void *self)
{
	try
	{
		static_cast<Engine *>(self)->mouseUp(button, x, y);
	}
	catch(const std::exception& e)
	{
		static_cast<Engine *>(self)->_error = std::current_exception();
		mlx_loop_end(static_cast<Engine *>(self)->_mlx);
	}
	return 0;
}

int		Engine::mouseMoveTrampoline(int x, int y, void *self)
{
	try
	{
		static_cast<Engine *>(self)->mouseMove(x, y);
	}
	catch(const std::exception& e)
	{
		static_cast<Engine *>(self)->_error = std::current_exception();
		mlx_loop_end(static_cast<Engine *>(self)->_mlx);
	}
	return 0;
}

int		Engine::onKeyDownTrampoline(int keycode, void *self)
{
	try
	{
		static_cast<Engine *>(self)->onKeyDown(keycode);
	}
	catch(const std::exception& e)
	{
		static_cast<Engine *>(self)->_error = std::current_exception();
		mlx_loop_end(static_cast<Engine *>(self)->_mlx);
	}
	return 0;
}

int		Engine::onKeyUpTrampoline(int keycode, void *self)
{
	try
	{
		static_cast<Engine *>(self)->onKeyUp(keycode);
	}
	catch(const std::exception& e)
	{
		static_cast<Engine *>(self)->_error = std::current_exception();
		mlx_loop_end(static_cast<Engine *>(self)->_mlx);
	}
	return 0;
}

void	Engine::mouseDown(int button, int x, int y)
{
	this->_screen->onMouseDown(button, x, y);
	for (auto &b : this->_themesButtons)
	{
		if (b.isMouseOn())
		{
			b.click();
			b.setPressed(false);
		}
	}
}

void	Engine::mouseUp(int button, int x, int y)
{
	this->_screen->onMouseUp(button, x, y);
}

void	Engine::mouseMove(int x, int y)
{
	for (auto &b : this->_themesButtons)
	{
		b.clearHover();
		b.hoverIfOnButton(x, y);
	}
	this->_screen->onMouseMove(x, y);
}

void	Engine::onKeyDown(int keycode)
{
	this->_screen->onKeyDown(keycode);
}

void	Engine::onKeyUp(int keycode)
{
	this->_screen->onKeyUp(keycode);
}

void	Engine::loop()
{
	this->_screen->update();
	this->_screen->render(this->_img);
	this->_themeRect.render(this->_img);
	for (auto &b : this->_themesButtons)
		b.render(this->_img);
	auto &text = this->_textures["Snake.xpm"];
	int w = this->_width;
	text.applyBloc(this->_img, std::make_pair(0, 0), w - 93, 12, 21.f / 16.f);
	text.applyBloc(this->_img, std::make_pair(13, 1), w - 93, 12, 21.f / 16.f);
	text.applyBloc(this->_img, std::make_pair(0, 7), w - 63, 12, 21.f / 16.f);
	text.applyBloc(this->_img, std::make_pair(13, 8), w - 65, 12, 21.f / 16.f);
	text.applyBloc(this->_img, std::make_pair(0, 14), w - 33, 12, 21.f / 16.f);
	text.applyBloc(this->_img, std::make_pair(13, 15), w - 33, 12, 21.f / 16.f);
	mlx_put_image_to_window(this->_mlx, this->_mlx_win, this->_img.getImg(), 0, 0);
	if (this->_nextScreen)
		this->_screen = std::move(this->_nextScreen);
}

void	Engine::prepareNextScreen(std::unique_ptr<IScreen> next)
{
	this->_nextScreen = std::move(next);
}

using MlxHook = int (*)();
using AnyFn   = void (*)();

template <typename F>
static MlxHook asMlxHook(F f)
{
    return reinterpret_cast<MlxHook>(reinterpret_cast<AnyFn>(f));
}

void	Engine::run(void)
{
	try
	{
		mlx_hook(this->_mlx_win, ON_KEYDOWN, (1L << 0), asMlxHook(&Engine::onKeyDownTrampoline), this);
		mlx_hook(this->_mlx_win, ON_KEYUP, (1L << 1), asMlxHook(&Engine::onKeyUpTrampoline), this);
		mlx_hook(this->_mlx_win, ON_MOUSEDOWN, (1L << 2), asMlxHook(&Engine::mouseDownTrampoline), this);
		mlx_hook(this->_mlx_win, ON_MOUSEUP, (1L << 3), asMlxHook(&Engine::mouseUpTrampoline), this);
		mlx_hook(this->_mlx_win, ON_MOUSEMOVE, (1L << 6), asMlxHook(&Engine::mouseMoveTrampoline), this);
		mlx_loop_hook(this->_mlx, asMlxHook(&Engine::loopTrampoline), this);
		mlx_loop(this->_mlx);
		if (this->_error)
			std::rethrow_exception(this->_error);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
	}
}

void	Engine::quit(void)
{
	mlx_loop_end(this->_mlx);
}