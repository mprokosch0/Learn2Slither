#ifndef GAME_HPP

# define GAME_HPP

# include <iostream>
# include <string>
# include <array>
# include <cstdlib>
# include <vector>

# define INT_MIN -2147483648

enum class Actions
{
	FORWARD,
	LEFT,
	RIGHT
};

enum class Directions
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

enum class GridValues
{
	EMPTY,
	WALL,
	SNAKE_BODY,
	SNAKE_HEAD,
	GREEN_APPLE,
	RED_APPLE,
};

class Snake
{
	private:
		std::vector<std::pair<int, int>>					_pos;
		size_t												_initSize;
		Directions											_orientation;
		static	std::array<std::array<Directions, 3>, 4>	_orienTable;
	
	public:
		Snake(size_t size = 3);
		Snake(Snake const &rhs);
		~Snake();
	
	public:
		Snake	&operator=(Snake const &rhs);
	
	public:
		void									addSegment(int x, int y);
		void									removeSegment(void);
		void									determineOrient(void);
		size_t									getInitSize(void) const;
		size_t									size(void) const;
		const std::pair<int, int>				&getHeadPos(void) const;
		const std::pair<int, int>				&getTailPos(void) const;
		const std::vector<std::pair<int, int>>	&getPos(void) const;
		const Directions						&getOrientation(void) const;
		void									move(Actions dir);
		void									reset(void);

};

class Board
{
	private:
    	std::vector<std::vector<GridValues>>	_grid;
		size_t									_rows;
		size_t									_cols;
		Snake									_snake;
		bool									_lose;
		bool									_win;

	private:
		void	prepareGrid(void);
		void	genApple(bool apple);
		void	genSnake(void);

	
	public:
		Board(size_t rows = 10, size_t cols = 10);
		Board(Board const &rhs);
		~Board();
	
	public:
		Board							&operator=(Board const &rhs);
		const std::vector<GridValues>	&operator[](size_t index) const;
	
	public:
		const std::vector<std::vector<GridValues>>	&getGrid(void) const;
		const std::vector<std::vector<GridValues>>	getSnakeViews(void) const;
		const std::pair<int, int>					&getSnakeHeadPos(void) const;
		const Directions							&getSnakeOrientation(void) const;
		size_t										getSnakeSize(void) const;
		double										snakeMove(Actions dir);
		void										reset(void);
		bool										hasLost(void) const;
		bool										hasWon(void) const;
};

std::ostream	&operator<<(std::ostream &os, const Board &board);
std::ostream	&operator<<(std::ostream &os, const std::vector<std::vector<GridValues>> &views);

#endif