#include "learn2Slither.hpp"

SelectScreen::SelectScreen(size_t width, size_t height): IScreen(width, height)
{
	this->_buttons.emplace_back((box){20, 15, 150, 30}, 0x1A2113, 0x1A2113,
			0x3A4A2E, 0x8DB600, Text("Back", 0.8f, 0xE6EDD8),
			[width, height](Engine &engine)
			{
				engine.prepareNextScreen(std::make_unique<TitleScreen>(width, height));
			});
}

void	SelectScreen::update(void)
{}

void	SelectScreen::onKey(int keycode)
{
	(void)keycode;
}

void	SelectScreen::onMouseDown(int button, int x, int y, Engine &engine)
{
	(void)x, (void)y;
	if (button == M_LEFT_CLICK && !this->_hooks.mouseLeft)
	{
		for (Button &but : this->_buttons)
		{
			if (but.isMouseOn())
			{
				but.click(engine);
				break ;
			}
		}
	}
	updateHooks(button, HOOK_DOWN);
}

void	SelectScreen::onMouseUp(int button, int x, int y, Engine &engine)
{
	(void)x, (void)y, (void)engine;
	updateHooks(button, HOOK_UP);
}

void	SelectScreen::onMouseMove(int x, int y)
{
	for (Button &but : this->_buttons)
	{
		but.clearHover();
		but.hoverIfOnButton(x, y);
	}
}

void	SelectScreen::render(Image &img)
{
	img.colorImage(0x12170D);
	img.putRectangle((box){0, 0, static_cast<int>(this->_width), 60}, 0x1A2113);
	Text title("MODEL SELECTION", 1.3f, 0xF2F7E6);
	title.render(this->_width / 2, 30, img, Text::HAlign::Center, Text::VAlign::Center);
	for (Button &but : this->_buttons)
		but.render(img);
}