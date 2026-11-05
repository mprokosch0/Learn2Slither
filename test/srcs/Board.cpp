#include "test.hpp"

void	Board::genSnake(void)
{
	int x = (rand() % (this->_cols - 2)) + 1;
	int y = (rand() % (this->_rows - 2)) + 1;
	this->_grid[y][x] = GridValues::SNAKE_HEAD;
	this->_snake.addSegment(x, y);
	for (size_t i = 1; i < this->_snake.getInitSize(); i++)
	{
		std::vector<std::pair<int, int>> followup;
		if (x > 0 && this->_grid[y][x - 1] == GridValues::EMPTY)
			followup.emplace_back(x - 1, y);
		if (y > 0 && this->_grid[y - 1][x] == GridValues::EMPTY)
			followup.emplace_back(x, y - 1);
		if (x < (int)_cols - 1 && this->_grid[y][x + 1] == GridValues::EMPTY)
			followup.emplace_back(x + 1, y);
		if (y < (int)_rows - 1 && this->_grid[y + 1][x] == GridValues::EMPTY)
			followup.emplace_back(x, y + 1);
		int index = rand() % followup.size();
		x = followup[index].first;
		y = followup[index].second;
		this->_grid[y][x] = GridValues::SNAKE_BODY;
		this->_snake.addSegment(x, y);
	}
	this->_snake.determineOrient();
}

void	Board::genApple(bool apple)
{
	for (size_t i = 0; i < 1; i++)
	{
		int x = (rand() % (this->_cols - 2)) + 1;
		int y = (rand() % (this->_rows - 2)) + 1;
		if (this->_grid[y][x] == GridValues::EMPTY)
			this->_grid[y][x] = apple ? GridValues::RED_APPLE : GridValues::GREEN_APPLE;
		else
			i--;
	}
	
}

void	Board::prepareGrid(void)
{
	std::fill_n(this->_grid[0].begin(), this->_cols, GridValues::WALL);
	std::fill_n(this->_grid.back().begin(), this->_cols, GridValues::WALL);
	for (size_t i = 0; i < this->_grid.size(); i++)
	{
		this->_grid[i][0] = GridValues::WALL;
		this->_grid[i][this->_cols - 1] = GridValues::WALL;
	}
	genSnake();
	genApple(0);
	genApple(0);
	genApple(1);
}

Board::Board(size_t rows, size_t cols): _rows(rows), _cols(cols), _lose(false), _win(false)
{
	srand(time(NULL));
	if (rows < 10 || rows > 40)
		throw std::invalid_argument("Board: error: rows size must be between"
			 "10 and 40 (included) instead got: " + std::to_string(rows));
	if (cols < 10 || cols > 40)
		throw std::invalid_argument("Board: error: rows size must be between"
			 "10 and 40 (included) instead got: " + std::to_string(cols));
	this->_grid.assign(rows, std::vector<GridValues>(cols, GridValues::EMPTY));
	prepareGrid();
}

Board::Board(Board const &rhs): _grid(rhs._grid), _rows(rhs._rows), _cols(rhs._cols),
	_snake(rhs._snake), _lose(rhs._lose), _win(rhs._win)
{}

Board::~Board()
{}

Board	&Board::operator=(Board const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_grid = rhs._grid;
	this->_rows = rhs._rows;
	this->_cols = rhs._cols;
	this->_snake = rhs._snake;
	this->_lose = rhs._lose;
	this->_win = rhs._win;
	return *this;
}

const std::vector<GridValues>	&Board::operator[](size_t index) const
{
	if (index > this->_rows)
		throw std::invalid_argument("Board: error: index out of range");
	return this->_grid[index];
}

const std::vector<std::vector<GridValues>>	&Board::getGrid(void) const
{
	return this->_grid;
}

bool	Board::hasLost(void) const
{
	return this->_lose;
}

bool	Board::hasWon(void) const
{
	return this->_win;
}

const Directions	&Board::getSnakeOrientation(void) const
{
	return this->_snake.getOrientation();
}

const std::pair<int, int>	&Board::getSnakeHeadPos(void) const
{
	return this->_snake.getHeadPos();
}

size_t	Board::getSnakeSize(void) const
{
	return this->_snake.size();
}

const std::vector<std::vector<GridValues>>	Board::getSnakeViews(void) const
{
	std::vector<std::vector<GridValues>> res;
	int x = this->_snake.getHeadPos().first;
	int y = this->_snake.getHeadPos().second;
	Directions	dir = this->_snake.getOrientation();
	std::vector<GridValues>	column, row;

	res.reserve(4);
	row = this->_grid[y];
	for (size_t i = 0; i < this->_rows; i++)
		column.push_back(this->_grid[i][x]);

	std::vector<GridValues> north(column.rbegin() + (column.size() - y), column.rend());
	std::vector<GridValues> south(column.begin() + y + 1, column.end());
	std::vector<GridValues> east(row.begin() + x + 1, row.end());
	std::vector<GridValues> west(row.rbegin() + (row.size() - x), row.rend());

	if (dir == Directions::NORTH) // convention clockwise : Front, right, behind, left
		res = {north, east, south, west};
	else if (dir == Directions::EAST)
		res = {east, south, west, north};
	else if (dir == Directions::SOUTH)
		res = {south, west, north, east};
	else if (dir == Directions::WEST)
		res = {west, north, east, south};
	return res;
}

double	Board::snakeMove(Actions dir)
{
	const std::pair<int, int>	tPos = this->_snake.getTailPos();
	this->_snake.move(dir);
	const std::vector<std::pair<int, int>>	&snakePos = this->_snake.getPos();
	int x = snakePos[0].first;
	int y = snakePos[0].second;
	double reward = -0.1;

	if (this->_grid[y][x] != GridValues::GREEN_APPLE)
		this->_grid[tPos.second][tPos.first] = GridValues::EMPTY;
	if (this->_grid[y][x] == GridValues::WALL || this->_grid[y][x] == GridValues::SNAKE_BODY)
	{
		this->_lose = true;
		reward = -100.0;
	}
	else if (this->_grid[y][x] == GridValues::GREEN_APPLE)
	{
		this->_snake.addSegment(tPos.first, tPos.second);
		this->_grid[y][x] = GridValues::SNAKE_HEAD;
		if (this->_snake.size() > 1)
			this->_grid[snakePos[1].second][snakePos[1].first] = GridValues::SNAKE_BODY;
		genApple(0);
		reward = 10.0;
	}
	else if (this->_grid[y][x] == GridValues::RED_APPLE)
	{
		this->_grid[y][x] = GridValues::SNAKE_HEAD;
		if (this->_snake.size() > 1)
			this->_grid[snakePos[1].second][snakePos[1].first] = GridValues::SNAKE_BODY;
		this->_grid[snakePos.back().second][snakePos.back().first] = GridValues::EMPTY;
		this->_snake.removeSegment();
		genApple(1);
		reward = -10.0;
	}
	else if (this->_grid[y][x] == GridValues::EMPTY)
	{
		this->_grid[y][x] = GridValues::SNAKE_HEAD;
		if (this->_snake.size() > 1)
			this->_grid[snakePos[1].second][snakePos[1].first] = GridValues::SNAKE_BODY;
	}
	if (!this->_lose && this->_snake.size() == 10)
	{
		this->_win = true;
		reward = 100.0;
	}
	else if (!this->_snake.size())
	{
		this->_lose = true;
		reward = -100.0;
	}
	return reward;
}

void	Board::reset(void)
{
	this->_snake.reset();
	for (std::vector<GridValues> &line : this->_grid)
		std::fill_n(line.begin(), line.size(), GridValues::EMPTY);
	prepareGrid();
	this->_lose = false;
	this->_win = false;
}

static std::string getGridVal(GridValues val)
{
	switch (val)
	{
		case GridValues::EMPTY:
			return "\033[90m0\033[0m";
		
		case GridValues::WALL:
			return "W";
		
		case GridValues::SNAKE_BODY:
			return "S";
		
		case GridValues::SNAKE_HEAD:
			return "\033[34mH\033[0m";
		
		case GridValues::GREEN_APPLE:
			return "\033[32mG\033[0m";
		
		case GridValues::RED_APPLE:
			return "\033[31mR\033[0m";
	
		default:
			return "\033[90m0\033[0m";
	}
}

std::ostream &operator<<(std::ostream &os, const Board &board)
{
	const std::vector<std::vector<GridValues>> &grid  = board.getGrid();
	const std::pair<int, int> &hPos = board.getSnakeHeadPos();

	const int x = hPos.first;
	const int y = hPos.second;


	for (size_t i = 0; i < grid.size(); i++)
	{
		os << '[';
		for (size_t j = 0; j < grid[i].size(); j++)
		{
			if ((int)j == x || (int)i == y)
				os << getGridVal(grid[i][j]);
			else
				os << ' ';
			if (j + 1 != grid[i].size())
				os << " ";
		}
		os << ']';
		if (i + 1 != grid.size())
			os << std::endl;
	}
	return os;
}

std::ostream	&operator<<(std::ostream &os, const std::vector<std::vector<GridValues>> &views)
{
	
	for (size_t i = 0; i < views.size(); i++)
	{
		os << '[';
		for (size_t j = 0; j < views[i].size(); j++)
		{
			os << getGridVal(views[i][j]);
			if (j + 1 != views[i].size())
				os << " ";
		}
		os << ']' << std::endl;
		if (i + 1 != views.size())
			os << std::endl;
	}
	return os;
}