#include "Agent.hpp"

Interpreter::Interpreter(): _danger({0}), _snakeBody({0}), _greenApple({0}),
	_redApple({0}), _reward(0), _action(Actions::FORWARD),
	_win(false), _lose(false)
{}

Interpreter::Interpreter(const std::vector<std::vector<GridValues>> &views,
	double reward, Actions action,
	bool win, bool lose):
	_danger({0}), _snakeBody({0}), _greenApple({0}), _redApple({0}),
	_reward(reward), _action(action), _win(win), _lose(lose)
{
	if (views.size())
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < views[i].size(); j++)
			{
				GridValues val = views[i][j];
				int correct = (i == 3) ? -1 : 0;
				if (val == GridValues::WALL)
				{
					if (i != 2)
					{
						if (j == 0)
							this->_danger[i + correct] = 3;
						else if (j <= 2)
							this->_danger[i + correct] = 2;
						else
							this->_danger[i + correct] = 1;
						break ;
					}
				}
				else if (val == GridValues::SNAKE_BODY)
				{
					if (i != 2)
					{
						if (j == 0)
							this->_snakeBody[i + correct] = 3;
						else if (j <= 2)
							this->_snakeBody[i + correct] = 2;
						else
							this->_snakeBody[i + correct] = 1;
					}
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
}

Interpreter::Interpreter(Interpreter const &rhs): _danger(rhs._danger),
		_snakeBody(rhs._snakeBody), _greenApple(rhs._greenApple),
		_redApple(rhs._redApple), _reward(rhs._reward), _action(rhs._action),
		_win(rhs._win), _lose(rhs._lose)
{}

Interpreter::~Interpreter()
{}

Interpreter	&Interpreter::operator=(Interpreter const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_danger = rhs._danger;
	this->_snakeBody = rhs._snakeBody;
	this->_greenApple = rhs._greenApple;
	this->_redApple = rhs._redApple;
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
	for (size_t j = 0; j < this->_snakeBody.size(); j++)
	{
		res |= this->_snakeBody[j] << i;
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
	return res;
}