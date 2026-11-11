#include "learn2Slither.hpp"

TitleScreen::TitleScreen(size_t width, size_t height, Engine &engine): IScreen(width, height)
{
	this->_buttons.emplace_back((box){(static_cast<int>(width) / 2) - 100,
			(static_cast<int>(height) / 2) + 75, 200, 50}, 0x407560, 0x407560,
			0x0, 0xffffff, Text("Play", 1.f, 0x0, 0x0), true,
			[width, height, &engine]()
			{
				engine.prepareNextScreen(std::make_unique<SelectScreen>(width, height, engine));
			});
	this->_buttons.emplace_back((box){(static_cast<int>(width) / 2) - 100,
			(static_cast<int>(height) / 2) + 150, 200, 50}, 0x407560, 0x407560,
			0x0, 0xffffff, Text("Quit", 1.f, 0x0, 0x0), true,
			[&engine]()
			{
				engine.quit();
			});
}

void	TitleScreen::update(void)
{}

void	TitleScreen::onKey(int keycode)
{
	(void)keycode;
}

void	TitleScreen::onMouseDown(int button, int x, int y)
{
	(void)x, (void)y;
	if (button == M_LEFT_CLICK && !this->_hooks.mouseLeft)
	{
		for (Button &but : this->_buttons)
		{
			if (but.isMouseOn())
			{
				but.click();
				break ;
			}
		}
	}
	updateHooks(button, HOOK_DOWN);
}

void	TitleScreen::onMouseUp(int button, int x, int y)
{
	(void)x, (void)y;
	updateHooks(button, HOOK_UP);
}

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
	img.colorImage(0x33aa33);
	for (Button &but : this->_buttons)
		but.render(img);
}