#ifndef TEST_HPP

#define TEST_HPP

# include <iostream>
# include <string>
# include <algorithm>
# include <array>
# include <cstring>
# include <unistd.h>
# include <unordered_map>
# include <map>
# include <sstream>
# include <fstream>
# include <vector>
# include <cstdlib>

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
		std::vector<std::pair<int, int>>	_pos;
		size_t								_initSize;
		Directions							_orientation;
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
    	std::vector<std::vector<GridValues>> _grid;
		size_t								_rows;
		size_t								_cols;
		Snake								_snake;
		bool								_lose;
		bool								_win;

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

class Interpreter
{
	private:
		std::array<int, 4>	_danger; 		//if danger is adjacent on the direction
		std::array<int, 4>	_greenApple;	//if green apple in direction
		std::array<int, 4>	_redApple;		//if red apple in direction
		std::array<bool, 2>	_snakeSize;		//diff value depending the snake length
		double				_reward;
		Actions				_action;
		bool				_win;
		bool				_lose;

	public:
		Interpreter(void);
		Interpreter(const std::vector<std::vector<GridValues>> &views, size_t snakeSize,
						double reward, Actions action, bool win, bool lose);
		Interpreter(Interpreter const &rhs);
		~Interpreter();
	
	public:
		Interpreter	&operator=(Interpreter const &rhs);

	public:
		int		getState(void) const;
		double	getReward(void) const;
		Actions	getAction(void) const;
		bool	isTerminal(void) const;
};

class Agent
{
	private:
		std::unordered_map<int, std::array<double, 3>>	_QTable;
		int												_state;
		float											_learningRate;
		float											_discountFactor;
		float											_expRate;
		u_int32_t										_gamesPlayed;
		int												_nbWin;
		float											_winRatio;
		bool											_training;
	
	private:
		void	parseModel(std::ifstream &file);
	

	public:
		Agent(void);
		Agent(double learningRate, double discountFactor, double epsilon);
		Agent(Agent const &rhs);
		~Agent();
	
	public:
		bool				lastWasRandom;
		Agent	&operator=(Agent const &rhs);
	
	public:
		void	setState(int state);
		int 	getState(void) const;
		void	setTraining(bool training);
		void	incrementTotalGames(bool win);
		float	getWinRatio(void);
		int		getGamePlayed(void) const;
		float	getExplRate(void) const;
		void	updateQTable(Interpreter &i);
		void 	decayExploration(void);
		Actions	chooseAction(void);
		void	exportModel(std::string name);
		void	importModel(std::string model);

};

std::ostream	&operator<<(std::ostream &os, const Board &board);
std::ostream	&operator<<(std::ostream &os, const std::vector<std::vector<GridValues>> &views);

#endif
