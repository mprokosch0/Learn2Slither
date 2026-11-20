#include "learn2Slither.hpp"

#define MAX_PIXEL_WIDTH 891
#define MAX_PIXEL_HEIGHT 777
#define PIXEL_PAD_BOARD 8

GameScreen::GameScreen(size_t width, size_t height, Engine &engine,
	Board board, Agent agent, unsigned int nbGames,
	bool visualDisplay, std::string exportName):
	IScreen(width, height), _board(board), _agent(agent), _nbGames(nbGames),
	_visualDisplay(visualDisplay), _boardZoom(0.f), _exportName(exportName),
	_snakeText(engine.getTextures().at("Snake.xpm")), _theme(engine.getTheme()),
	_stepTime(0.3), _step(), _boardRect(), _stats(), _options(), _logs()
{
	(void)engine;
	int cellSize = std::min(MAX_PIXEL_WIDTH / this->_board.getWidth(), MAX_PIXEL_HEIGHT / this->_board.getHeight());
	this->_boardZoom = static_cast<float>(cellSize) / this->_snakeText.getBlockSize().first;
	this->_step = std::chrono::steady_clock::now();
	const Theme &t = THEMES[this->_theme % 3];
	const std::vector<std::vector<GridValues>> &grid = this->_board.getGrid();
	const std::pair<int, int> &bs = this->_snakeText.getBlockSize();
	this->_boardRect = Rectangle((box){PIXEL_PAD_BOARD, PIXEL_PAD_BOARD,
				(int)((int)grid[0].size() * bs.second * this->_boardZoom) + 14, (int)this->_height - PIXEL_PAD_BOARD * 2},
				t.panel, 2, t.borderBox);

	initStats(t);
	initOptions(t);
	initLogs(t);
}

void	GameScreen::initStats(const Theme &t)
{
	Stats &s = this->_stats;
	const box &bRect = this->_boardRect.getDimentions();
	s.statsRect = Rectangle((box){bRect.x + bRect.w + PIXEL_PAD_BOARD, PIXEL_PAD_BOARD,
				(int)this->_width - PIXEL_PAD_BOARD - (bRect.x + bRect.w + PIXEL_PAD_BOARD), 200},
				t.panel, 2, t.borderBox);
}

void	GameScreen::initOptions(const Theme &t)
{
	Options &o = this->_options;
	const box &bRect = this->_boardRect.getDimentions();
	o.optionsRect = Rectangle((box){bRect.x + bRect.w + PIXEL_PAD_BOARD, 218,
				(int)this->_width - PIXEL_PAD_BOARD - (bRect.x + bRect.w + PIXEL_PAD_BOARD), 200},
				t.panel, 2, t.borderBox);
}

void	GameScreen::initLogs(const Theme &t)
{
	Logs &l = this->_logs;
	const box &bRect = this->_boardRect.getDimentions();
	l.logsRect = Rectangle((box){bRect.x + bRect.w + PIXEL_PAD_BOARD, 428,
				(int)this->_width - PIXEL_PAD_BOARD - (bRect.x + bRect.w + PIXEL_PAD_BOARD), 364},
				t.panel, 2, t.borderBox);
}

void	GameScreen::update(void)
{
	if (std::chrono::duration<double>(std::chrono::steady_clock::now() - this->_step).count() < this->_stepTime)
		return ;
	this->_step = std::chrono::steady_clock::now();
	Actions act = this->_agent.chooseAction();
	double reward = this->_board.snakeMove(act);
	Interpreter t(this->_board.getSnakeViews(), reward, act,
							this->_board.hasWon(), this->_board.hasLost());
	this->_agent.updateQTable(t);
	if (this->_board.hasLost())
	{
		this->_agent.decayExploration();
		this->_agent.incrementTotalGames();
		this->_board.reset();
		this->_agent.setState(Interpreter(this->_board.getSnakeViews(),
				0, Actions::FORWARD, 0, 0).getState());
	}
}

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

void	GameScreen::renderSnake(Image &img) const
{
	Directions dir = this->_board.getSnakeOrientation();
	std::pair<int, int> bSize = this->_snakeText.getBlockSize();
	float zoom = this->_boardZoom;
	bSize.first *= zoom;
	bSize.second *= zoom;
	std::array<int, 4>	snakeOrientText({0, 3, 2, 1});
	auto &snake = this->_board.getSnakePos();
	if (!snake.size())
		return ;
	auto &grid = this->_board.getGrid();
	const int yStart = (this->_height - grid.size() * bSize.first) / 2;
	auto putSnakeText = [&](size_t snakeIndex, const std::pair<int, int> &textCo)
							{
								this->_snakeText.applyBloc(img, textCo,
									(snake[snakeIndex].first) * bSize.second + PIXEL_PAD_BOARD + 7,
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

void	GameScreen::renderBoard(Image &img) const
{
	auto &grid = this->_board.getGrid();
	std::pair<int, int> bSize = this->_snakeText.getBlockSize();
	const float zoom = this->_boardZoom;
	bSize.first *= zoom;
	bSize.second *= zoom;
	const int pxWidth = grid[0].size() * bSize.second;
	const int pxHeight = grid.size() * bSize.first;
	const int yStart = (this->_height - pxHeight) / 2;

	this->_boardRect.render(img);

	for (int y = yStart; y < yStart + pxHeight; y += bSize.first)
	{
		for (int x = PIXEL_PAD_BOARD + 7; x < pxWidth + PIXEL_PAD_BOARD; x += bSize.second)
		{
			const GridValues &val = grid[(y - yStart) / bSize.first][(x - PIXEL_PAD_BOARD - 7) / bSize.second];
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

void	GameScreen::renderStats(Image &img) const
{
	const Stats &s = this->_stats;
	s.statsRect.render(img);
}

void	GameScreen::renderOptions(Image &img) const
{
	const Options &o = this->_options;
	o.optionsRect.render(img);
}

void	GameScreen::renderLogs(Image &img) const
{
	const Logs &l = this->_logs;
	l.logsRect.render(img);
}

void	GameScreen::render(Image &img)
{
	const Theme &t = THEMES[this->_theme % 3];
	img.colorImage(t.windowBg);
	renderStats(img);
	renderOptions(img);
	renderLogs(img);
	renderBoard(img);
	renderSnake(img);
}