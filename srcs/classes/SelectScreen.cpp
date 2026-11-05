#include "learn2Slither.hpp"

SelectScreen::SelectScreen(size_t width, size_t height): IScreen(width, height)
{
	this->_buttons.emplace_back((box){30, 15, 150, 30}, 0x1A2113, 0x8DB600,
			0x8DB600, 0x8DB600, Text("Back", 1.f, 0xC5E063, 0x12170D), true,
			[width, height](Engine &engine)
			{
				engine.prepareNextScreen(std::make_unique<TitleScreen>(width, height));
			});
	this->_buttons.emplace_back((box){991, (int)this->_height - 60, 150, 40},
			0x8DB600, 0xA8D41F, 0x8DB600, 0x8DB600,
			Text("Launch", 1.f, 0x12170D, 0x12170D), true,
			[](Engine &engine)
			{
				(void)engine;
			});

	this->_fileListRect = Rectangle((box){30, 125, 651, 595}, 0x1F281A, 2, 0x3A4A2E);

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

box		clip(const box &button, const Rectangle &rectangle)
{
	box rect = rectangle.getDimentions();
	int	bWidth = rectangle.getBorderWidth();
	int x1 = std::max(button.x, rect.x + bWidth);
	int y1 = std::max(button.y, rect.y + bWidth);
	int x2 = std::min(button.x + button.w, rect.x + rect.w - bWidth);
	int y2 = std::min(button.y + button.h, rect.y + rect.h - bWidth);
	return (box){x1, y1, std::max(0, x2 - x1), std::max(0, y2 - y1)};
}

static std::string	getFilename(std::string path)
{
	std::string filename = path;
	size_t dot = filename.rfind('.');
	filename = std::string(filename.begin(), filename.begin() + dot);
	size_t slash = filename.rfind('/');
	filename = std::string(filename.begin() + slash + 1, filename.end());
	return filename;
}

static int	isInVec(std::string &filename, const std::vector<Button> &buttons)
{
	for (size_t i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].getText().getText() == filename)
			return i;
	}
	return -1;
}

void	SelectScreen::addEntry(void)
{
	std::filesystem::directory_iterator it("./models");
	for (auto const &dirEntry : it)
	{
		std::string ext = dirEntry.path();
		size_t index = ext.rfind('.');
		ext = std::string(ext.begin() + index, ext.end());
		std::string filename = getFilename(dirEntry.path());
		if (!std::filesystem::is_regular_file(dirEntry)
			|| ext != ".txt" || isInVec(filename, this->_files) != -1)
			continue ;
		if (!this->_files.size())
		{
			this->_files.emplace_back((box){40, 135 + (int)this->_files.size() * 50, 250, 40},
				0x283322, 0x2F3C28, 0x4a5a3a, 0x6B8E00,
				Text(filename, 1.f, 0xE6EDD8, 0xC5E063), true,
				[](Engine &engine)
				{
					(void)engine;
				}
				);
		}
		else
		{
			box ldims = this->_files.back().getDimentions();
			this->_files.emplace_back((box){40, ldims.y + ldims.h + 10, 250, 40},
				0x283322, 0x2F3C28, 0x4a5a3a, 0x6B8E00,
				Text(filename, 1.f, 0xE6EDD8, 0xC5E063), true,
				[](Engine &engine)
				{
					(void)engine;
				}
				);
		}
	}
}

void	SelectScreen::removeEntry(void)
{
	for (size_t i = 0; i < this->_files.size(); i++)
	{
		std::filesystem::directory_iterator it("./models");
		bool find = false;
		for (auto const &dirEntry : it)
		{
			std::string ext = dirEntry.path();
			size_t index = ext.rfind('.');
			ext = std::string(ext.begin() + index, ext.end());
			std::string filename = getFilename(dirEntry.path());
			if (!std::filesystem::is_regular_file(dirEntry)
				|| ext != ".txt")
				continue ;
			if (filename == this->_files[i].getText().getText())
			{
				find = true;
				break ;
			}
		}
		if (find == false)
		{
			this->_files.erase(this->_files.begin() + i);
			break ;
		}
	}	
}

void	SelectScreen::updateFilesVector(void)
{
	std::filesystem::directory_iterator it("./models");

	size_t nbEntry = 0;
	for (auto const &dirEntry : it)
	{
		std::string ext = dirEntry.path();
		size_t index = ext.rfind('.');
		ext = std::string(ext.begin() + index, ext.end());

		if (std::filesystem::is_regular_file(dirEntry) && ext == ".txt")
			nbEntry++;
	}
	if (nbEntry == this->_files.size())
		return ;
	if (nbEntry > this->_files.size())
		addEntry();
	else
		removeEntry();
}

void	SelectScreen::update(void)
{
	updateFilesVector();
}

void	SelectScreen::onKey(int keycode)
{
	(void)keycode;
}

#define SCROLLSIZE 15

void	SelectScreen::scrollButtonsFiles(int button)
{
	box	rectDim = this->_fileListRect.getDimentions();
	if (button == M_SCROLL_UP)
	{
		box dim1 = this->_files[0].getDimentions();
		rectDim.y += 10;
		if (dim1.y < rectDim.y)
		{
			int increment = (dim1.y + SCROLLSIZE < rectDim.y) ? SCROLLSIZE : rectDim.y - dim1.y;
			for (Button &but : this->_files)
			{
				box dims = but.getDimentions();
				dims.y += increment;
				but.setDimentions(dims);
			}
		}
		return ;
	}
	box ldim = this->_files.back().getDimentions();
	rectDim.h -= 10;
	if (ldim.y + ldim.h > rectDim.y + rectDim.h)
	{
		int decrement = (ldim.y + ldim.h - SCROLLSIZE > rectDim.y + rectDim.h)
			? SCROLLSIZE : (ldim.y + ldim.h) - (rectDim.y + rectDim.h);
		for (Button &but : this->_files)
		{
			box dims = but.getDimentions();
			dims.y -= decrement;
			but.setDimentions(dims);
		}
	}
	return ;
}

void	SelectScreen::onMouseDown(int button, int x, int y, Engine &engine)
{
	(void)x, (void)y;
	for (Button &but : this->_buttons)
	{
		but.clearHover();
		but.hoverIfOnButton(x, y);
	}
	for (Button &file : this->_files)
	{
		file.clearHover();
		box cliped = clip(file.getDimentions(), this->_fileListRect);
		file.hoverIfOnButtonClip(x, y, cliped);
	}
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
	if (button == M_SCROLL_DOWN || button == M_SCROLL_UP)
	{
		box rectDims = this->_fileListRect.getDimentions();
		if (x >= rectDims.x && x < rectDims.x + rectDims.w
			&& y >= rectDims.y && y < rectDims.y + rectDims.h)
		{
			int nbButtonsMin = (rectDims.h - 10) / 50;
			if ((int)this->_files.size() > nbButtonsMin)
				scrollButtonsFiles(button);
		}
	}
	updateHooks(button, HOOK_DOWN);
}

void	SelectScreen::onMouseUp(int button, int x, int y, Engine &engine)
{
	(void)x, (void)y, (void)engine;
	for (Button &but : this->_buttons)
	{
		but.clearHover();
		but.hoverIfOnButton(x, y);
	}
	for (Button &file : this->_files)
	{
		file.clearHover();
		box cliped = clip(file.getDimentions(), this->_fileListRect);
		file.hoverIfOnButtonClip(x, y, cliped);
	}
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
		box cliped = clip(file.getDimentions(), this->_fileListRect);
		file.hoverIfOnButtonClip(x, y, cliped);
	}
}

void	SelectScreen::renderScrollList(Image &img) const
{
	for (const Button &but : this->_files)
	{
		box butDims = but.getDimentions();
		box clipedBox = clip(butDims, this->_fileListRect);
		if (clipedBox.w == 0 || clipedBox.h == 0)
			continue ;
		
		but.renderClip(img, clipedBox);
	}
}

void	SelectScreen::render(Image &img)
{
	img.colorImage(0x12170D);
	this->_fileListRect.render(img);
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