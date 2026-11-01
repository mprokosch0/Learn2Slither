#include "learn2Slither.hpp"

Engine::Engine(size_t winWidth, size_t winHeight): _width(winWidth),
	_height(winHeight), _nextScreen(nullptr), _error(nullptr)
{
	try
	{
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
		this->_screen = std::make_unique<TitleScreen>(this->_width, this->_height);
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
	this->_img.destroy();
	mlx_destroy_window(this->_mlx, this->_mlx_win);
	mlx_destroy_display(this->_mlx);
	free(this->_mlx);
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

void	Engine::mouseDown(int button, int x, int y)
{
	this->_screen->onMouseDown(button, x, y, *this);
}

void	Engine::mouseUp(int button, int x, int y)
{
	this->_screen->onMouseUp(button, x, y, *this);
}

void	Engine::mouseMove(int x, int y)
{
	this->_screen->onMouseMove(x, y);
}

void	Engine::loop()
{
	this->_screen->render(this->_img);
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