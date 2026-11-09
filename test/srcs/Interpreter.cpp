#include "test.hpp"

Interpreter::Interpreter(): _danger({0}), _greenApple({0}),
	_redApple({0}), _snakeSize({false}), _reward(0), _action(Actions::FORWARD),
	_win(false), _lose(false)
{}

Interpreter::Interpreter(const std::vector<std::vector<GridValues>> &views,
	size_t snakeSize, double reward, Actions action,
	bool win, bool lose):
	_danger({0}), _greenApple({0}), _redApple({0}),
	_snakeSize({false}), _reward(reward), _action(action), _win(win), _lose(lose)
{
	if (views.size())
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < views[i].size(); j++)
			{
				GridValues val = views[i][j];
				if (val == GridValues::WALL || val == GridValues::SNAKE_BODY
					|| (snakeSize == 1 && val == GridValues::RED_APPLE))
				{
					if (j == 0)
						this->_danger[i] = 3;
					else if (j <= 2)
						this->_danger[i] = 2;
					else
						this->_danger[i] = 1;
					break ;
				}
				else if (val == GridValues::GREEN_APPLE && !this->_greenApple[i])
				{
					if (j == 0)
						this->_greenApple[i] = 3;
					else if (j <= 2)
						this->_greenApple[i] = 2;
					else
						this->_greenApple[i] = 1;
				}
				else if (val == GridValues::RED_APPLE && !this->_redApple[i])
				{
					if (j == 0)
						this->_redApple[i] = 3;
					else if (j <= 2)
						this->_redApple[i] = 2;
					else
						this->_redApple[i] = 1;
				}
			}
		}
	}

	if (snakeSize <= 1)
		;
	else if (snakeSize >= 2 && snakeSize < 5)
		this->_snakeSize[0] = true;
	else if (snakeSize >= 5 && snakeSize < 9)
		this->_snakeSize[1] = true;
	else
	{
		this->_snakeSize[0] = true;
		this->_snakeSize[1] = true;
	}
}

Interpreter::Interpreter(Interpreter const &rhs): _danger(rhs._danger),
		_greenApple(rhs._greenApple), _redApple(rhs._redApple),
		_snakeSize(rhs._snakeSize), _reward(rhs._reward), _action(rhs._action),
		 _win(rhs._win), _lose(rhs._lose)
{}

Interpreter::~Interpreter()
{}

Interpreter	&Interpreter::operator=(Interpreter const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_danger = rhs._danger;
	this->_greenApple = rhs._greenApple;
	this->_redApple = rhs._redApple;
	this->_snakeSize = rhs._snakeSize;
	this->_reward = rhs._reward;
	this->_action = rhs._action;
	this->_win = rhs._win;
	this->_lose = rhs._lose;
	return *this;
}

double	Interpreter::getReward(void) const
{
	return this->_reward;
}

Actions	Interpreter::getAction(void) const
{
	return this->_action;
}

bool	Interpreter::isTerminal(void) const
{
	return (this->_win || this->_lose);
}

int		Interpreter::getState(void) const
{
	int res(0);
	int i = 0;
	for (size_t j = 0; j < this->_danger.size(); j++)
	{
		res |= this->_danger[j] << i;
		i += 2;
	}
	for (size_t j = 0; j < this->_greenApple.size(); j++)
	{
		res |= this->_greenApple[j] << i;
		i += 2;
	}
	for (size_t j = 0; j < this->_redApple.size(); j++)
	{
		res |= this->_redApple[j] << i;
		i += 2;
	}
	for (size_t j = 0; j < this->_snakeSize.size(); j++)
		res |= this->_snakeSize[j] << i++;
	return res;
}