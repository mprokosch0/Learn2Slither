#include "test.hpp"

Agent::Agent(void):	_state(0), _learningRate(0.1f), _discountFactor(0.92f),
	_expRate(0.9f), _gamesPlayed(0), _training(true)
{}

Agent::Agent(double lrRate, double disFactor, double epsilon):	_state(0),
	_learningRate(lrRate), _discountFactor(disFactor), _expRate(epsilon),
	_gamesPlayed(0), _training(true)
{}

Agent::Agent(Agent const &rhs): _QTable(rhs._QTable), _state(rhs._state),
	_learningRate(rhs._learningRate), _discountFactor(rhs._discountFactor),
	_expRate(rhs._expRate), _gamesPlayed(rhs._gamesPlayed),
	_training(rhs._training)
{}

Agent::~Agent()
{}

Agent	&Agent::operator=(Agent const &rhs)
{
	if (this == &rhs)
		return *this;
	this->_QTable = rhs._QTable;
	this->_expRate = rhs._expRate;
	this->_learningRate = rhs._learningRate;
	this->_discountFactor = rhs._discountFactor;
	this->_state = rhs._state;
	this->_gamesPlayed = rhs._gamesPlayed;
	this->_training = rhs._training;
	return *this;
}

void	Agent::incrementTotalGames(void)
{
	if (this->_training)
		this->_gamesPlayed++;
}

int		Agent::getGamePlayed(void) const
{
	return this->_gamesPlayed;
}

void	Agent::setTraining(bool training)
{
	this->_training = training;
}

bool	Agent::isTraining(void) const
{
	return this->_training;
}

void	Agent::setState(int state)
{
	this->_state = state;
}

int		Agent::getState(void) const
{
	return 	this->_state;
}

float	Agent::getExplRate(void) const
{
	if (this->_training)
		return this->_expRate;
	else
		return 0.f;
}

void	Agent::updateQTable(Interpreter &i)
{
	int	state = i.getState();
	if (this->_training)
	{
		if (this->_QTable.find(state) == this->_QTable.end())
			this->_QTable.emplace(state, std::array{0., 0., 0.});
		
		if (this->_QTable.find(this->_state) == this->_QTable.end())
			this->_QTable.emplace(this->_state, std::array{0., 0., 0.});

		std::array<double, 3> &currentQLine = this->_QTable.at(this->_state);
		std::array<double, 3> &newQLine = this->_QTable.at(state);
		double &currentQVal = currentQLine[static_cast<size_t>(i.getAction())];
		double	maxVal = std::max(newQLine[0], std::max(newQLine[1], newQLine[2]));

		double target = i.getReward();
		if (!i.isTerminal())
			target += this->_discountFactor * maxVal;
		
		currentQVal = (1 - this->_learningRate) * currentQVal + this->_learningRate * target;
	}
	this->_state = state;
}

static Actions getMaxAction(const std::array<double, 3> &line)
{
	double max = std::max(line[0], std::max(line[1], line[2]));
	std::array<size_t, 3> best;
	size_t n = 0;
	for (size_t i = 0; i < 3; i++)
		if (line[i] == max)
			best[n++] = i;
	return static_cast<Actions>(best[rand() % n]);
}

void Agent::decayExploration(void)
{
	if (this->_training)
    	this->_expRate = std::max(0.01f, this->_expRate * 0.9999f);
}

Actions	Agent::chooseAction(void)
{
	float randNb = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float rate = (this->_training) ? this->_expRate : 0.0;
	if (randNb < rate)
		return static_cast<Actions>(rand() % 3);
	if (this->_QTable.find(this->_state) != this->_QTable.end())
	{
		const std::array<double, 3> &stateLine = this->_QTable.at(this->_state);
		return getMaxAction(stateLine);
	}
	return static_cast<Actions>(rand() % 3);
}

void	readBytes(std::ifstream &file, void *dst, std::size_t n)
{
	file.read(reinterpret_cast<char *>(dst), n);
	if (!file)
		throw std::runtime_error("Error: corrupted file");
}

void	Agent::parseModel(std::ifstream &file)
{
	std::string buffer;
	
	readBytes(file, &this->_gamesPlayed, sizeof(this->_gamesPlayed));
	readBytes(file, &this->_learningRate, sizeof(this->_learningRate));
	readBytes(file, &this->_discountFactor, sizeof(this->_discountFactor));
	readBytes(file, &this->_expRate, sizeof(this->_expRate));
	int inBetween = 0;
	int	state = 0;
	std::array<double, 3> line;
	while (1)
	{
		readBytes(file, &state, sizeof(state));
		if (state == -1)
			break;
		readBytes(file, line.data(), sizeof(double) * 3);
		
		readBytes(file, &inBetween, sizeof(inBetween));
		if (inBetween != INT_MIN)
			throw std::runtime_error("Error: corrupted file");
		this->_QTable.emplace(state, line);
	}
}

void	Agent::importModel(std::string model)
{
	if (model.rfind(".") == std::string::npos)
		throw std::invalid_argument("Error: ext should be .mdl");
	std::string ext(model.begin() + model.rfind("."), model.end());
	if (ext != ".mdl")
		throw std::invalid_argument("Error: ext should be .mdl");
	std::ifstream fd;
	fd.open(model, std::ios::binary);
	if (!fd.is_open())
		throw std::runtime_error("Error: could not open the file");
	parseModel(fd);
	fd.close();
}

void	Agent::exportModel(std::string name)
{
	if (name.empty())
		return ;
	std::ofstream fd;
	fd.open(name, std::ios::binary);
	if (!fd.is_open())
		throw std::invalid_argument("Error: could not create file: " + name);
	fd.write(reinterpret_cast<const char *>(&this->_gamesPlayed), sizeof(this->_gamesPlayed));
	fd.write(reinterpret_cast<const char *>(&this->_learningRate), sizeof(this->_learningRate));
	fd.write(reinterpret_cast<const char *>(&this->_discountFactor), sizeof(this->_discountFactor));
	fd.write(reinterpret_cast<const char *>(&this->_expRate), sizeof(this->_expRate));
	int inBetween = INT_MIN;
	int end = -1;
	for (auto &pair : this->_QTable)
	{
		std::array<double, 3> line = pair.second;
		fd.write(reinterpret_cast<const char *>(&pair.first), sizeof(pair.first));
	
		fd.write(reinterpret_cast<const char *>(&line[0]), sizeof(line[0]));
		fd.write(reinterpret_cast<const char *>(&line[1]), sizeof(line[1]));
		fd.write(reinterpret_cast<const char *>(&line[2]), sizeof(line[2]));

		fd.write(reinterpret_cast<const char *>(&inBetween), sizeof(inBetween));
	}
	fd.write(reinterpret_cast<const char *>(&end), sizeof(end));
	fd.close();
}