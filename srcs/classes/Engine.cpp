#include "learn2Slither.hpp"

Engine::Engine(size_t winWidth, size_t winHeight): _width(winWidth), _height(winHeight)
{
	try
	{
		this->_mlx = mlx_init();
		if (!this->_mlx)
			throw std::runtime_error("Mlx_init function failed");
		this->_mlx_win = mlx_new_window(this->_mlx, this->_width, this->_height, std::string("Learn2Slither").data());
		if (!this->_mlx)
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
		std::cerr << e.what() << '\n'; //need to clean
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
	static_cast<Engine *>(self)->loop();
	return 0;
}

int		Engine::mouseMoveTrampoline(int x, int y, void *self)
{
	static_cast<Engine *>(self)->mouseMove(x, y);
	return 0;
}

void	Engine::mouseMove(int x, int y)
{
	this->_screen->onMouseMove(x, y);
}

void	Engine::loop()
{
	this->_screen->render(this->_img);
	mlx_put_image_to_window(this->_mlx, this->_mlx_win, this->_img.getImg(), 0, 0);	
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
		mlx_hook(this->_mlx_win, ON_MOUSEMOVE, (1L << 6), asMlxHook(&Engine::mouseMoveTrampoline), this);
		mlx_loop_hook(this->_mlx, asMlxHook(&Engine::loopTrampoline), this);
		mlx_loop(this->_mlx);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n'; //need to clean
	}
}