#include "test.hpp"

Agent::Agent(void)
{}

Agent::Agent(Agent const &rhs): _QTable(rhs._QTable)
{}

Agent::~Agent()
{}

Agent	&Agent::operator=(Agent const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_QTable = rhs._QTable;
	return *this;
}

void	Agent::setState(int state)
{
	this->_state = state;
}

void	Agent::updateQTable(Interpreter &i)
{
	int	state = i.getState();
	if (this->_QTable.find(state) == this->_QTable.end())
		this->_QTable.emplace(state, std::array{0., 0., 0.});
	std::array<double, 3> &currentQLine = this->_QTable.at(state);
	double currentQVal = currentQLine[static_cast<size_t>(i.getAction())];

}