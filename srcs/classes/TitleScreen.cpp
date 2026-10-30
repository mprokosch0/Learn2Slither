#include "learn2Slither.hpp"

TitleScreen::TitleScreen(size_t width, size_t height): IScreen(width, height)
{
	this->_buttons.emplace_back((box){(static_cast<int>(width) / 2) - 400,
			(static_cast<int>(height) / 2) + 75, 800, 50}, 0x407560, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-+,;:!?.*&#%");
	// this->_buttons.emplace_back((box){(static_cast<int>(width) / 2) - 100,
	// 		(static_cast<int>(height) / 2) + 150, 200, 50}, 0x407560, "Docteur9!?");
}

void	TitleScreen::update(void)
{}

void	TitleScreen::onKey(int keycode){(void)keycode;}

void	TitleScreen::onMouseMove(int x, int y)
{
	for (Button &but : this->_buttons)
	{
		but.clearHover();
		but.hoverIfOnButton(x, y);
	}
}

void	TitleScreen::render(Image &img)
{
	for (Button &but : this->_buttons)
		but.render(img);
}