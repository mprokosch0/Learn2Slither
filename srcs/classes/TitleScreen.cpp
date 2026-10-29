#include "learn2Slither.hpp"

TitleScreen::TitleScreen(size_t width, size_t height): IScreen(width, height)
{}

void	TitleScreen::update(void)
{}

void	TitleScreen::onKey(int keycode){(void)keycode;}

void	TitleScreen::render(Image &img)
{
	img.putPixel(this->_width - 1, this->_width - 1, 0xff0000);
	img.putPixel(0, 0, 0x0000ff);
}