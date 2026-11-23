#include "learn2Slither.hpp"

TitleScreen::TitleScreen(size_t width, size_t height, Engine &engine): IScreen(width, height),
	_theme(engine.getTheme())
{
	this->_buttons.emplace_back((box){(static_cast<int>(width) / 2) - 100,
			(static_cast<int>(height) / 2) + 75, 200, 50}, 0x407560, 0x407560,
			0x0, 0xffffff, 0x407560, 0x407560, Text("Play", 1.f, 0x0, 0x0), true,
			[width, height, &engine]()
			{
				engine.prepareNextScreen(std::make_unique<SelectScreen>(width, height, engine));
			});
	this->_buttons.emplace_back((box){(static_cast<int>(width) / 2) - 100,
			(static_cast<int>(height) / 2) + 150, 200, 50}, 0x407560, 0x407560,
			0x0, 0xffffff, 0x407560, 0x407560, Text("Quit", 1.f, 0x0, 0x0), true,
			[&engine]()
			{
				engine.quit();
			});
}

void	TitleScreen::updateTheme(void)
{}

void	TitleScreen::update(void)
{}

void	TitleScreen::onKeyDown(int keycode)
{
	updateKeyHooks(static_cast<keycodes>(keycode), HOOK_DOWN);
}

void	TitleScreen::onKeyUp(int keycode)
{
	updateKeyHooks(static_cast<keycodes>(keycode), HOOK_UP);
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
	updateMouseHooks(button, HOOK_DOWN);
}

void	TitleScreen::onMouseUp(int button, int x, int y)
{
	(void)x, (void)y;
	updateMouseHooks(button, HOOK_UP);
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
	const Theme &t = THEMES[this->_theme % 3];
	img.colorImage(t.windowBg);
	for (Button &but : this->_buttons)
		but.render(img);
}