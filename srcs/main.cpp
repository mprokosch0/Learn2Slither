#include "learn2Slither.hpp"

int main()
{
	try
	{
		Engine engine(1422, 800);
		engine.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}