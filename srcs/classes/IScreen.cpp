#include "learn2Slither.hpp"

void	IScreen::updateHooks(int button, bool downUp)
{
	bool update = (downUp) ? false : true;
	switch (button)
	{
		case M_LEFT_CLICK:
			this->_hooks.mouseLeft = update;
			break;
		
		case M_SCROLL_BUTTON:
			this->_hooks.mouseRight = update;
			break;

		case M_RIGHT_CLICK:
			this->_hooks.mouseScrollButton = update;
			break;
		
		case M_SCROLL_UP:
			this->_hooks.mouseScrollUp = update;
			break;
		
		case M_SCROLL_DOWN:
			this->_hooks.mouseScrollDown = update;
			break;
		
		default:
			break;
	}
}