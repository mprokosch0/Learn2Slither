#include "learn2Slither.hpp"

SelectScreen::SelectScreen(size_t width, size_t height): IScreen(width, height)
{
	this->_buttons.emplace_back((box){30, 15, 150, 30}, 0x1A2113, 0x8DB600,
			0x8DB600, 0x8DB600, Text("Back", 1.f, 0xC5E063, 0x12170D),
			[width, height](Engine &engine)
			{
				engine.prepareNextScreen(std::make_unique<TitleScreen>(width, height));
			});
	this->_buttons.emplace_back((box){991, (int)this->_height - 60, 150, 40},
			0x8DB600, 0xA8D41F, 0x8DB600, 0x8DB600,
			Text("Launch", 1.f, 0x12170D, 0x12170D),
			[](Engine &engine)
			{
				(void)engine;
			});

	this->_fileList = Rectangle((box){30, 125, 651, 595}, 0x1F281A, 2, 0x3A4A2E);

	this->_title = std::make_pair(
		Text("MODEL SELECTION", 1.3f, 0xF2F7E6, 0xF2F7E6),
		std::array<int, 2>{(int)this->_width / 2, 30});

	this->_texts.emplace_back(std::make_pair(
		Text("MODELS LIST", 1.2f, 0xC5E063, 0xC5E063),
		std::array<int, 2>{30, 75}));

	this->_texts.emplace_back(std::make_pair(
		Text("SETTINGS", 1.2f, 0xC5E063, 0xC5E063),
		std::array<int, 2>{((int)this->_width / 2) + 30, 75}));

	this->_texts.emplace_back(std::make_pair(
		Text("Choose a model :", 1.f, 0x9AA88A, 0x9AA88A),
		std::array<int, 2>{35, 115}));

	this->_texts.emplace_back(std::make_pair(
		Text("Configure the session :", 1.f, 0x9AA88A, 0x9AA88A),
		std::array<int, 2>{746, 115}));
	
	this->_rectangles.emplace_back(
		(box){0, 0, static_cast<int>(this->_width), 60}, 0x1A2113, 2, 0x3A4A2E);
	this->_rectangles.emplace_back(
		(box){((int)this->_width / 2) + 30, 125, 651, 595}, 0x1F281A, 2, 0x3A4A2E);
}

void	SelectScreen::update(void)
{
	std::filesystem::directory_iterator it("./models");

	size_t nbEntry = 0;
	for (auto const &dirEntry : it)
	{
		if (std::filesystem::is_regular_file(dirEntry))
			nbEntry++;
	}
	if (nbEntry == this->_files.size())
		return ;
	this->_files.clear();
	it = std::filesystem::directory_iterator("./models");
	for (auto const &dirEntry : it)
	{
		if (!std::filesystem::is_regular_file(dirEntry))
			continue ;
		std::string filename = dirEntry.path();
		size_t dot = filename.rfind('.');
		filename = std::string(filename.begin(), filename.begin() + dot);
		size_t slash = filename.rfind('/');
		filename = std::string(filename.begin() + slash + 1, filename.end());
		this->_files.emplace_back((box){40, 135 + (int)this->_files.size() * 50, 250, 40},
			0x283322, 0x2F3C28, 0x3A4A2E, 0x6B8E00, Text(filename, 1.f, 0xE6EDD8, 0xC5E063),
		[](Engine &engine)
		{
			(void)engine;
		}
		);
	}
}

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
		for (Button &file : this->_files)
		{
			if (file.isMouseOn())
			{
				file.click(engine);
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
	for (Button &file : this->_files)
	{
		file.clearHover();
		file.hoverIfOnButton(x, y);
	}
}

box		clip(box &button, box &rect)
{
	int x1 = std::max(button.x, rect.x);
	int y1 = std::max(button.y, rect.y);
	int x2 = std::min(button.x + button.w, rect.x + rect.w);
	int y2 = std::min(button.y + button.h, rect.y + rect.h);
	return (box){x1, y1, std::max(0, x2 - x1), std::max(0, y2 - y1)};
}

void	SelectScreen::renderScrollList(Image &img) const
{
	box rectDims = this->_fileList.getDimentions();
	for (const Button &but : this->_files)
	{
		box butDims = but.getDimentions();
		box clipedBox = clip(butDims, rectDims);
		if (clipedBox.w == 0 || clipedBox.h == 0)
			continue ;
		
		but.render(img);
	}
}

void	SelectScreen::render(Image &img)
{
	img.colorImage(0x12170D);
	this->_fileList.render(img);
	for (Rectangle &rect : this->_rectangles)
		rect.render(img);
	
	this->_title.first.render(this->_title.second[0], this->_title.second[1], img,
		Text::HAlign::Center, Text::VAlign::Center);

	for (auto &pair : this->_texts)
		pair.first.render(pair.second[0], pair.second[1], img);

	img.putVLine(this->_width / 2, 60, this->_height, 0x3A4A2E, 3);

	for (Button &but : this->_buttons)
		but.render(img);
	
	renderScrollList(img);
}