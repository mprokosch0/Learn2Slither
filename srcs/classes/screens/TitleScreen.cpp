#include "learn2Slither.hpp"

TitleScreen::TitleScreen(size_t width, size_t height, Engine &engine): IScreen(width, height),
	_theme(engine.getTheme()), _b(15, 15), _a(), _snakeText(engine.getTextures().at("Snake.xpm")),
	_tPoint(std::chrono::steady_clock::now())
{
	this->_a.importModel("./models/t3.mdl");
	const Theme &t = THEMES[this->_theme % 3];
	this->_buttons.emplace_back((box){(static_cast<int>(width) / 2) - 100,
			(static_cast<int>(height) / 2) + 75, 200, 50}, t.inner, t.hover,
			t.borderBtn, t.borderHover, t.hover, t.borderHover,
			Text("Play", 1.f, t.textMain, t.textMain), true,
			[width, height, &engine]()
			{
				engine.prepareNextScreen(std::make_unique<SelectScreen>(width, height, engine));
			});
	this->_buttons.emplace_back((box){(static_cast<int>(width) / 2) - 100,
			(static_cast<int>(height) / 2) + 150, 200, 50}, t.inner, t.hover,
			t.borderBtn, t.borderHover, t.hover, t.borderHover,
			Text("Quit", 1.f, t.textMain, t.textMain), true,
			[&engine]()
			{
				engine.quit();
			});
}

void	TitleScreen::updateTheme(void)
{
	const Theme &t = THEMES[this->_theme % 3];
	for (auto &b : this->_buttons)
		b.setColors(t.inner, t.hover, t.borderBtn, t.borderHover,
			t.hover, t.borderHover, t.textMain, t.textMain);
}

void	TitleScreen::update(void)
{
	if (std::chrono::duration<double>(std::chrono::steady_clock::now() - this->_tPoint).count() < 0.10)
		return ;
	this->_tPoint = std::chrono::steady_clock::now();
	Actions act = this->_a.chooseAction();
	double reward = this->_b.snakeMove(act);
	Interpreter t(this->_b.getSnakeViews(), reward, act,
							this->_b.hasWon(), this->_b.hasLost());
	this->_a.updateQTable(t);
	if (this->_b.hasLost())
	{
		this->_b.reset();
		this->_a.setState(Interpreter(this->_b.getSnakeViews(),
				0, Actions::FORWARD, 0, 0).getState());
	}
}

void	TitleScreen::onKeyDown(int keycode)
{
	updateKeyHooks(static_cast<keycodes>(keycode), HOOK_DOWN);
}

void	TitleScreen::onKeyUp(int keycode)
{
	updateKeyHooks(static_cast<keycodes>(keycode), HOOK_UP);
}

void	TitleScreen::onMouseDown(int button, int x, int y)
{
	(void)x, (void)y;
	if (button == M_LEFT_CLICK && !this->_hooks.mouseLeft)
	{
		for (Button &but : this->_buttons)
		{
			if (but.isMouseOn())
			{
				but.click();
				break ;
			}
		}
	}
	updateMouseHooks(button, HOOK_DOWN);
}

void	TitleScreen::onMouseUp(int button, int x, int y)
{
	(void)x, (void)y;
	updateMouseHooks(button, HOOK_UP);
}

void	TitleScreen::onMouseMove(int x, int y)
{
	for (Button &but : this->_buttons)
	{
		but.clearHover();
		but.hoverIfOnButton(x, y);
	}
}

void	TitleScreen::renderSnake(Image &img) const
{
	Directions dir = this->_b.getSnakeOrientation();
	std::pair<int, int> bSize = this->_snakeText.getBlockSize();
	float zoom = 6.f;
	bSize.first *= zoom;
	bSize.second *= zoom;
	std::array<int, 4>	snakeOrientText({0, 3, 2, 1});
	auto &snake = this->_b.getSnakePos();
	if (!snake.size())
		return ;
	auto &grid = this->_b.getGrid();
	const int xStart = (this->_width - grid[0].size() * bSize.second) / 2;
	const int yStart = (this->_height - grid.size() * bSize.first) / 2;
	auto putSnakeText = [&](size_t snakeIndex, const std::pair<int, int> &textCo)
							{
								this->_snakeText.applyBloc(img, textCo,
									(snake[snakeIndex].first) * bSize.second + xStart,
									yStart + (snake[snakeIndex].second) * bSize.first,
									zoom);
							};
	putSnakeText(0, std::make_pair(0, 3 + snakeOrientText[static_cast<int>(dir)] + 7 * this->_theme));
	for (size_t i = 1; i < snake.size() - 1; i++)
	{
		int xBdiff = snake[i].first - snake[i - 1].first;
		int yBdiff = snake[i].second - snake[i - 1].second;
		int xAdiff = snake[i + 1].first - snake[i].first;
		int yAdiff = snake[i + 1].second - snake[i].second;
		if ((xBdiff < 0 && yAdiff < 0)
			|| (yBdiff > 0 && xAdiff > 0))
			putSnakeText(i, std::make_pair(3, 2 + 7 * this->_theme));
		else if ((xBdiff < 0 && yAdiff > 0)
			|| (yBdiff < 0 && xAdiff > 0))
			putSnakeText(i, std::make_pair(5, 2 + 7 * this->_theme));
		else if ((xBdiff < 0 && !yAdiff)
			|| (xBdiff > 0 && !yAdiff))
			putSnakeText(i, std::make_pair(1, 2 + 7 * this->_theme));
		else if ((yBdiff < 0 && xAdiff < 0)
			|| (xBdiff > 0 && yAdiff > 0))
			putSnakeText(i, std::make_pair(4, 2 + 7 * this->_theme));
		else if ((yBdiff < 0 && !xAdiff)
			|| (yBdiff > 0 && !xAdiff))
			putSnakeText(i, std::make_pair(0, 2 + 7 * this->_theme));
		else if ((xBdiff > 0 && yAdiff < 0)
			|| (yBdiff > 0 && xAdiff < 0))
			putSnakeText(i, std::make_pair(2, 2 + 7 * this->_theme));
	}
	if (snake.size() > 1)
	{
		int xdiff = snake.back().first - snake[snake.size() - 2].first;
		int ydiff = snake.back().second - snake[snake.size() - 2].second;
		if (xdiff < 0)
			putSnakeText(snake.size() - 1, std::make_pair(9, 2 + 7 * this->_theme));
		else if (xdiff > 0)
			putSnakeText(snake.size() - 1, std::make_pair(7, 2 + 7 * this->_theme));
		else if (ydiff < 0)
			putSnakeText(snake.size() - 1, std::make_pair(8, 2 + 7 * this->_theme));
		else if (ydiff > 0)
			putSnakeText(snake.size() - 1, std::make_pair(6, 2 + 7 * this->_theme));
	}
}

void	TitleScreen::renderBoard(Image &img) const
{
	auto &grid = this->_b.getGrid();
	std::pair<int, int> bSize = this->_snakeText.getBlockSize();
	const float zoom = 6.f;
	bSize.first *= zoom;
	bSize.second *= zoom;
	const int pxWidth = grid[0].size() * bSize.second;
	const int pxHeight = grid.size() * bSize.first;
	const int xStart = (this->_width - pxWidth) / 2;
	const int yStart = (this->_height - pxHeight) / 2;

	for (int y = yStart; y < yStart + pxHeight; y += bSize.first)
	{
		for (int x = xStart; x < xStart + pxWidth; x += bSize.second)
		{
			const GridValues &val = grid[(y - yStart) / bSize.first][(x - xStart) / bSize.second];
			this->_snakeText.applyBloc(img, std::make_pair(0, 1 + 7 * this->_theme), x, y, zoom);
			switch (val)
			{	
				case GridValues::GREEN_APPLE:
					this->_snakeText.applyBloc(img, std::make_pair(1, 21), x, y, zoom);
					break;
				
				case GridValues::RED_APPLE:
					this->_snakeText.applyBloc(img, std::make_pair(0, 21), x, y, zoom);
					break;
				
				case GridValues::WALL:
					this->_snakeText.applyBloc(img, std::make_pair(13, 1 + 7 * this->_theme), x, y, zoom);
					break;
			
				default:
					break;
			}
		}
	}
}

void	TitleScreen::render(Image &img)
{
	const Theme &t = THEMES[this->_theme % 3];
	img.colorImage(t.windowBg);
	renderBoard(img);
	renderSnake(img);
	for (Button &but : this->_buttons)
		but.render(img);
}