#include "learn2Slither.hpp"

#define MAX_PIXEL_WIDTH 891
#define MAX_PIXEL_HEIGHT 777
#define PIXEL_PAD_BOARD 8

GameScreen::GameScreen(size_t width, size_t height, Engine &engine,
	Board board, Agent agent, unsigned int nbGames,
	bool visualDisplay, std::string exportName):
	IScreen(width, height), _board(board), _agent(agent), _nbGames(nbGames), _gameCount(0),
	_visualDisplay(visualDisplay), _boardZoom(0.f), _exportName(exportName),
	_snakeText(engine.getTextures().at("Snake.xpm")), _theme(engine.getTheme()),
	_stepTime(1.0), _step(), _boardRect(), _stats(), _options(), _logs()
{
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
	int leftX = bRect.x + bRect.w + PIXEL_PAD_BOARD;
	int rightX = leftX + (int)this->_width - PIXEL_PAD_BOARD - leftX;
	s.statsRect = CornerLessRect((box){leftX, PIXEL_PAD_BOARD,
				(int)this->_width - PIXEL_PAD_BOARD - leftX, 200},
				t.panel, 2, t.borderBox, (uint8_t)CornerLessRect::Corners::TOP_RIGHT,
				(box){0, 0, 95, 35});
	
	s.title = std::make_pair(
		Text("Statistics", 1.2f, t.title, t.title),
		std::array<int, 2>({leftX + 5, PIXEL_PAD_BOARD + 10}));
	
	s.names.emplace_back(std::make_pair(
		Text("Game Count", 1.f, t.textMain, t.textMain),
		std::array<int, 2>({leftX + 5, s.title.second[1] + 45})
	));
	s.names.emplace_back(std::make_pair(
		Text("Nb reached 10", 1.f, t.textMain, t.textMain),
		std::array<int, 2>({leftX + 5, s.names.back().second[1] + 25})
	));
	s.names.emplace_back(std::make_pair(
		Text("Snake max size", 1.f, t.textMain, t.textMain),
		std::array<int, 2>({leftX + 5, s.names.back().second[1] + 25})
	));
	s.names.emplace_back(std::make_pair(
		Text("Snake average size", 1.f, t.textMain, t.textMain),
		std::array<int, 2>({leftX + 5, s.names.back().second[1] + 25})
	));
	s.names.emplace_back(std::make_pair(
		Text("Max tick duration", 1.f, t.textMain, t.textMain),
		std::array<int, 2>({leftX + 5, s.names.back().second[1] + 25})
	));
	s.names.emplace_back(std::make_pair(
		Text("Average tick duration", 1.f, t.textMain, t.textMain),
		std::array<int, 2>({leftX + 5, s.names.back().second[1] + 25})
	));

	s.values.emplace_back(std::make_pair(
		Text(std::to_string(this->_gameCount), 1.f, t.textSecondary, t.textSecondary),
		std::array<int, 2>({rightX - 5, s.title.second[1] + 45})
	));
	s.values.emplace_back(std::make_pair(
		Text(std::to_string(s.nbReached10), 1.f, t.textSecondary, t.textSecondary),
		std::array<int, 2>({rightX - 5, s.values.back().second[1] + 25})
	));
	s.values.emplace_back(std::make_pair(
		Text(std::to_string(s.snakeMaxSize), 1.f, t.textSecondary, t.textSecondary),
		std::array<int, 2>({rightX - 5, s.values.back().second[1] + 25})
	));
	s.values.emplace_back(std::make_pair(
		Text(std::format("{:.2f}", s.avgSnakeSize), 1.f, t.textSecondary, t.textSecondary),
		std::array<int, 2>({rightX - 5, s.values.back().second[1] + 25})
	));
	s.values.emplace_back(std::make_pair(
		Text(std::to_string(s.maxTickDuration), 1.f, t.textSecondary, t.textSecondary),
		std::array<int, 2>({rightX - 5, s.values.back().second[1] + 25})
	));
	s.values.emplace_back(std::make_pair(
		Text(std::format("{:.2f}", s.avgTickDuration), 1.f, t.textSecondary, t.textSecondary),
		std::array<int, 2>({rightX - 5, s.values.back().second[1] + 25})
	));
}

void	GameScreen::initOptions(const Theme &t)
{
	Options &o = this->_options;
	const box &bRect = this->_boardRect.getDimentions();
	this->_options.tickSpeed = 1;
	int leftX = bRect.x + bRect.w + PIXEL_PAD_BOARD;
	int w = (int)this->_width - PIXEL_PAD_BOARD - leftX;
	o.optionsRect = Rectangle((box){leftX, 218, w, 100}, t.panel, 2, t.borderBox);
	o.title = std::make_pair(
		Text("Options", 1.2f, t.title, t.title),
		std::array<int, 2>({leftX + 5, 228}));
	
	o.names.emplace_back(std::make_pair(
		Text("Tick speed", 1.f, t.textMain, t.textMain),
		std::array<int, 2>({leftX + 5, o.title.second[1] + 45})
	));

	o.values.emplace_back(std::make_pair(
		Text(std::to_string(o.tickSpeed) + "/sec", 1.f, t.textSecondary, t.textSecondary),
		std::array<int, 2>({leftX + w - 10, o.names.back().second[1]})
	));

	o.tickSlider = Slider<int>((box){leftX + 10, o.names.back().second[1] + 25, w - 20, 3},
		t.borderBox, t.disabled, t.accent, t.textBright, t.borderBtn, t.borderHover, t.muted, t.muted, 1, 100);
}

void	GameScreen::initLogs(const Theme &t)
{
	Logsbox &l = this->_logs;
	const box &bRect = this->_boardRect.getDimentions();
	int leftX = bRect.x + bRect.w + PIXEL_PAD_BOARD;
	box lRect = {leftX, 328, (int)this->_width - PIXEL_PAD_BOARD - leftX, 464};
	l.logsRect = Rectangle(lRect, t.panel, 2, t.borderBox);
	l.title = std::make_pair(
		Text("Logs", 1.2f, t.title, t.title),
		std::array<int, 2>({leftX + 5, lRect.y + 10}));
	
	l.logs = Logs((box){leftX + 5, lRect.y + 35, lRect.w - 10, lRect.h - 40}, t.terminalBg, t.borderBox,
		t.logInfo, t.logSuccess, t.logWarn, t.logError);
}

void	GameScreen::updateTheme(void)
{
	const Theme &t = THEMES[this->_theme % 3];
	this->_boardRect.setColors(t.panel, t.borderBox);

	this->_stats.statsRect.setColors(t.panel, t.borderBox);
	this->_stats.title.first.setColors(t.title, t.title);
	for (auto &txt : this->_stats.names)
		txt.first.setColors(t.textMain, t.textMain);
	for (auto &txt : this->_stats.values)
		txt.first.setColors(t.textSecondary, t.textSecondary);

	this->_options.optionsRect.setColors(t.panel, t.borderBox);
	this->_options.title.first.setColors(t.title, t.title);
	for (auto &txt : this->_options.names)
		txt.first.setColors(t.textMain, t.textMain);
	for (auto &txt : this->_options.values)
		txt.first.setColors(t.textSecondary, t.textSecondary);
	this->_options.tickSlider.setColors(t.borderBox, t.disabled, t.accent, t.textBright, t.borderBtn, t.borderHover, t.muted, t.muted);

	this->_logs.logsRect.setColors(t.panel, t.borderBox);
	this->_logs.title.first.setColors(t.title, t.title);
	this->_logs.logs.setColors(t.terminalBg, t.borderBox, t.logInfo, t.logSuccess, t.logWarn, t.logError);
}

void	GameScreen::update(void)
{
	if (std::chrono::duration<double>(std::chrono::steady_clock::now() - this->_step).count() < this->_stepTime)
		return ;
	if (this->_gameCount < this->_nbGames)
	{
		this->_stats.nbTicks++;
		this->_step = std::chrono::steady_clock::now();
		Actions act = this->_agent.chooseAction();
		if (act == Actions::FORWARD)
			this->_logs.logs.addLog("Snake move forward", Logs::Colors::INFO);
		else if (act == Actions::LEFT)
			this->_logs.logs.addLog("Snake move left", Logs::Colors::INFO);
		else if (act == Actions::RIGHT)
			this->_logs.logs.addLog("Snake move right", Logs::Colors::INFO);
		double reward = this->_board.snakeMove(act);
		if (reward > 0)
			this->_logs.logs.addLog("Snake has eaten a green apple", Logs::Colors::SUCCESS);
		else if (reward == -10)
			this->_logs.logs.addLog("Snake has eaten a red apple", Logs::Colors::WARN);
		Interpreter t(this->_board.getSnakeViews(), reward, act,
								this->_board.hasWon(), this->_board.hasLost());
		this->_agent.updateQTable(t);
		if (this->_board.hasLost())
		{
			this->_logs.logs.addLog("Snake has lost", Logs::Colors::ERROR);
			this->_gameCount++;
			size_t sSize = this->_board.getSnakeSize();
			if (sSize >= 10)
				this->_stats.nbReached10++;
			this->_stats.avgSnakeSize += sSize;
			this->_stats.snakeMaxSize = std::max(this->_stats.snakeMaxSize, sSize);
			this->_stats.avgTickDuration += this->_stats.nbTicks;
			this->_stats.maxTickDuration = std::max(this->_stats.maxTickDuration, this->_stats.nbTicks);

			this->_stats.nbTicks = 0;
			this->_agent.decayExploration();
			this->_agent.incrementTotalGames();
			this->_board.reset();
			this->_agent.setState(Interpreter(this->_board.getSnakeViews(),
					0, Actions::FORWARD, 0, 0).getState());
			
			this->_stats.values[0].first.setText(std::to_string(this->_gameCount));
			this->_stats.values[1].first.setText(std::to_string(this->_stats.nbReached10));
			this->_stats.values[2].first.setText(std::to_string(this->_stats.snakeMaxSize));
			this->_stats.values[3].first.setText(std::format("{:.2f}", this->_stats.avgSnakeSize / (float)this->_gameCount));
			this->_stats.values[4].first.setText(std::to_string(this->_stats.maxTickDuration));
			this->_stats.values[5].first.setText(std::format("{:.2f}", this->_stats.avgTickDuration / (float)this->_gameCount));
		}
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

void	GameScreen::updateHover(int x, int y)
{
	this->_options.tickSlider.clearHover();
	this->_options.tickSlider.hoverIfOnSlider(x, y);
}

void	GameScreen::onMouseDown(int button, int x, int y)
{
	updateHover(x, y);
	if (this->_options.tickSlider.isMouseOn())
	{
		this->_options.tickSlider.click(x, this->_options.tickSpeed);
		this->_stepTime = 1.0 / this->_options.tickSpeed;
		this->_options.values[0].first.setText(std::to_string(this->_options.tickSpeed) + "/sec");
	}
	updateMouseHooks(button, HOOK_DOWN);
}

void	GameScreen::onMouseUp(int button, int x, int y)
{
	updateHover(x, y);
	updateMouseHooks(button, HOOK_UP);
}

void	GameScreen::onMouseMove(int x, int y)
{
	if (!this->_hooks.mouseLeft)
		updateHover(x, y);
	if (this->_hooks.mouseLeft)
	{
		if (this->_options.tickSlider.isMouseOn())
		{
			this->_options.tickSlider.click(x, this->_options.tickSpeed);
			this->_stepTime = 1.0 / this->_options.tickSpeed;
			this->_options.values[0].first.setText(std::to_string(this->_options.tickSpeed) + "/sec");
		}
	}
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
	s.title.first.render(s.title.second[0], s.title.second[1], img, Text::HAlign::Left, Text::VAlign::Top);
	for (auto &txt : s.names)
		txt.first.render(txt.second[0], txt.second[1], img, Text::HAlign::Left, Text::VAlign::Center);
	for (auto &txt : s.values)
		txt.first.render(txt.second[0], txt.second[1], img, Text::HAlign::Right, Text::VAlign::Center);
}

void	GameScreen::renderOptions(Image &img)
{
	Options &o = this->_options;
	o.optionsRect.render(img);
	o.title.first.render(o.title.second[0], o.title.second[1], img, Text::HAlign::Left, Text::VAlign::Top);
	for (auto &txt : o.names)
		txt.first.render(txt.second[0], txt.second[1], img, Text::HAlign::Left, Text::VAlign::Center);
	for (auto &txt : o.values)
		txt.first.render(txt.second[0], txt.second[1], img, Text::HAlign::Right, Text::VAlign::Center);
	o.tickSlider.render(img);
}

void	GameScreen::renderLogs(Image &img) const
{
	const Logsbox &l = this->_logs;
	l.logsRect.render(img);
	l.title.first.render(l.title.second[0], l.title.second[1], img, Text::HAlign::Left, Text::VAlign::Top);
	l.logs.render(img);
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