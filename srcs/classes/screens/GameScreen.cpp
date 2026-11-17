#include "learn2Slither.hpp"

GameScreen::GameScreen(size_t width, size_t height, Engine &engine,
	Board board, Agent agent, bool visualDisplay, std::string exportName):
	IScreen(width, height), _board(board), _agent(agent),
	_visualDisplay(visualDisplay), _exportName(exportName)
{
	(void)engine;
}

void	GameScreen::update(void)
{}

void	GameScreen::onKeyDown(int keycode)
{
	updateKeyHooks(static_cast<keycodes>(keycode), HOOK_DOWN);
}

void	GameScreen::onKeyUp(int keycode)
{
	updateKeyHooks(static_cast<keycodes>(keycode), HOOK_UP);
}

void	GameScreen::onMouseDown(int button, int x, int y)
{
	(void)x, (void)y;
	updateMouseHooks(button, HOOK_DOWN);
}

void	GameScreen::onMouseUp(int button, int x, int y)
{
	(void)x, (void)y;
	updateMouseHooks(button, HOOK_UP);
}

void	GameScreen::onMouseMove(int x, int y)
{
	(void)x, (void)y;
}

void	GameScreen::render(Image &img)
{
	img.colorImage(0x00aa33);
}