#include "learn2Slither.hpp"

GameScreen::GameScreen(size_t width, size_t height, Engine &engine,
	Board board, Agent agent, unsigned int nbGames,
	bool visualDisplay, std::string exportName):
	IScreen(width, height), _board(board), _agent(agent), _nbGames(nbGames),
	_visualDisplay(visualDisplay), _exportName(exportName), _snakeText(engine.getTextures().at("Snake.xpm"))
{
	(void)engine;
}

void	GameScreen::update(void)
{}

void	GameScreen::onKeyDown(int keycode)
{
	updateKeyHooks(static_cast<keycodes>(keycode), HOOK_DOWN);
}

void	GameScreen::onKeyUp(int keycode)
{
	updateKeyHooks(static_cast<keycodes>(keycode), HOOK_UP);
}

void	GameScreen::onMouseDown(int button, int x, int y)
{
	(void)x, (void)y;
	updateMouseHooks(button, HOOK_DOWN);
}

void	GameScreen::onMouseUp(int button, int x, int y)
{
	(void)x, (void)y;
	updateMouseHooks(button, HOOK_UP);
}

void	GameScreen::onMouseMove(int x, int y)
{
	(void)x, (void)y;
}

void	GameScreen::renderBoard(Image &img) const
{
	auto &grid = this->_board.getGrid();
	std::pair<int, int> bSize = this->_snakeText.getBlockSize();
	float zoom = 3.f;
	bSize.first *= zoom;
	bSize.second *= zoom;
	for (size_t y = 1; y < grid.size() - 1; y++)
	{
		for (size_t x = 1; x < grid[y].size() - 1; x++)
		{
			const GridValues &val = grid[y][x];
			int xCo = (x - 1) * bSize.second;
			int yCo = (y - 1) * bSize.first;
			this->_snakeText.applyBloc(img, std::make_pair(0, 8 + 7), xCo, yCo, zoom);
			switch (val)
			{	
				case GridValues::GREEN_APPLE:
					this->_snakeText.applyBloc(img, std::make_pair(1, 21), xCo, yCo, zoom);
					break;
				
				case GridValues::RED_APPLE:
					this->_snakeText.applyBloc(img, std::make_pair(0, 21), xCo, yCo, zoom);
					break;
			
				default:
					break;
			}
		}
	}
	Directions dir = this->_board.getSnakeOrientation();
	std::array<int, 4>	snakeOrientText({0, 3, 2, 1});
	auto &snake = this->_board.getSnakePos();
	if (!snake.size())
		return ;
	auto putSnakeText = [&](size_t snakeIndex, const std::pair<int, int> &textCo)
							{
								this->_snakeText.applyBloc(img, textCo,
									(snake[snakeIndex].first - 1) * bSize.second,
									(snake[snakeIndex].second - 1) * bSize.first, zoom);					
							};
	putSnakeText(0, std::make_pair(0, 10 + snakeOrientText[static_cast<int>(dir)]));
	for (size_t i = 1; i < snake.size() - 1; i++)
	{
		int xBdiff = snake[i].first - snake[i - 1].first;
		int yBdiff = snake[i].second - snake[i - 1].second;
		int xAdiff = snake[i + 1].first - snake[i].first;
		int yAdiff = snake[i + 1].second - snake[i].second;
		if ((xBdiff < 0 && yAdiff < 0)
			|| (yBdiff > 0 && xAdiff > 0))
			putSnakeText(i, std::make_pair(3, 9));
		else if ((xBdiff < 0 && yAdiff > 0)
			|| (yBdiff < 0 && xAdiff > 0))
			putSnakeText(i, std::make_pair(5, 9));
		else if ((xBdiff < 0 && !yAdiff)
			|| (xBdiff > 0 && !yAdiff))
			putSnakeText(i, std::make_pair(1, 9));
		else if ((yBdiff < 0 && xAdiff < 0)
			|| (xBdiff > 0 && yAdiff > 0))
			putSnakeText(i, std::make_pair(4, 9));
		else if ((yBdiff < 0 && !xAdiff)
			|| (yBdiff > 0 && !xAdiff))
			putSnakeText(i, std::make_pair(0, 9));
		else if ((xBdiff > 0 && yAdiff < 0)
			|| (yBdiff > 0 && xAdiff < 0))
			putSnakeText(i, std::make_pair(2, 9));
	}
	if (snake.size() > 1)
	{
		int xdiff = snake.back().first - snake[snake.size() - 2].first;
		int ydiff = snake.back().second - snake[snake.size() - 2].second;
		if (xdiff < 0)
			putSnakeText(snake.size() - 1, std::make_pair(9, 9));
		else if (xdiff > 0)
			putSnakeText(snake.size() - 1, std::make_pair(7, 9));
		else if (ydiff < 0)
			putSnakeText(snake.size() - 1, std::make_pair(8, 9));
		else if (ydiff > 0)
			putSnakeText(snake.size() - 1, std::make_pair(6, 9));
	}
}

void	GameScreen::render(Image &img)
{
	img.colorImage(0x00aa33);
	renderBoard(img);
}