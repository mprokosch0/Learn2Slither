#include "test.hpp"

Interpreter::Interpreter(): _danger({false}), _greenApple({false}),
	_redApple({false}), _snakeSize({false}), _reward(0)
{}

Interpreter::Interpreter(const std::vector<std::vector<GridValues>> &views,
	size_t snakeSize, double reward, Actions action): _danger({false}),
	_greenApple({false}), _redApple({false}), _snakeSize({false}),
	_reward(reward), _action(action)
{
	for (size_t i = 0; i < 3; i++)
	{
		int j = i;
		if (i == 2)
			j++;
		if (views[j][0] == GridValues::WALL || views[j][0] == GridValues::SNAKE_BODY)
			this->_danger[i] = true;
	}
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < views[i].size(); j++)
		{
			GridValues val = views[i][j];
			if (val == GridValues::GREEN_APPLE)
				this->_greenApple[i] = true;
			else if (val == GridValues::RED_APPLE)
				this->_redApple[i] = true;
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

Interpreter::Interpreter(Interpreter const &rhs): _danger(rhs._danger), _greenApple(rhs._greenApple),
		_redApple(rhs._redApple), _snakeSize(rhs._snakeSize), _reward(rhs._reward)
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

int		Interpreter::getState(void) const
{
	int res(0);
	int i = 0;
	for (size_t j = 0; j < this->_danger.size(); j++)
		res |= this->_danger[j] << i++;
	for (size_t j = 0; j < this->_greenApple.size(); j++)
		res |= this->_greenApple[j] << i++;
	for (size_t j = 0; j < this->_redApple.size(); j++)
		res |= this->_redApple[j] << i++;
	for (size_t j = 0; j < this->_snakeSize.size(); j++)
		res |= this->_snakeSize[j] << i++;
	return res;
}