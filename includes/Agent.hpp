#ifndef AGENT_HPP

# define AGENT_HPP

# include <unordered_map>
# include <fstream>
# include "Game.hpp"


class Interpreter
{
	private:
		std::array<int, 3>	_danger; 		//if danger is on the direction
		std::array<int, 3>	_snakeBody;		//if snkaeBody is on the direction
		std::array<int, 4>	_greenApple;	//if green apple in direction
		std::array<int, 4>	_redApple;		//if red apple in direction
		double				_reward;
		Actions				_action;
		bool				_win;
		bool				_lose;

	public:
		Interpreter(void);
		Interpreter(const std::vector<std::vector<GridValues>> &views,
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
		std::string										_name;
		std::unordered_map<int, std::array<double, 3>>	_QTable;
		int												_state;
		float											_learningRate;
		float											_discountFactor;
		float											_epsilon;
		u_int32_t										_gamesPlayed;
		bool											_training;
	
	private:
		void	parseModel(std::ifstream &file);
	

	public:
		Agent(void);
		Agent(std::string path, std::string name);
		Agent(double learningRate, double discountFactor, double epsilon,
				std::string name);
		Agent(Agent const &rhs);
		~Agent();
	
	public:
		Agent	&operator=(Agent const &rhs);
	
	public:
		void				setState(int state);
		int 				getState(void) const;
		const std::string	&getName(void) const;
		void				setTraining(bool training);
		bool				isTraining(void) const;
		void				incrementTotalGames(void);
		int					getGamePlayed(void) const;
		size_t				getQTableSize(void) const;
		size_t				getQTableN0Entries(void) const;
		float				getLrRate(void) const;
		float				getDiscFacor(void) const;
		float				getEpsilon(void) const;
		void				updateQTable(Interpreter &i);
		void 				decayExploration(void);
		Actions				chooseAction(void);
		void				exportModel(std::string name);
		void				importModel(std::string model);

};

#endif