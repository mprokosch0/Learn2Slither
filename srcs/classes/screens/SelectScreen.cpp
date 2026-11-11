#include "learn2Slither.hpp"

SelectScreen::SelectScreen(size_t width, size_t height, Engine &engine): IScreen(width, height),
	_selectedFileSize(0)
{
	this->_buttons.emplace_back((box){30, 15, 150, 30}, 0x1A2113, 0x8DB600,
			0x8DB600, 0x8DB600, Text("Back", 1.f, 0xC5E063, 0x12170D), true,
			[width, height, &engine]()
			{
				engine.prepareNextScreen(std::make_unique<TitleScreen>(width, height, engine));
			});
	this->_buttons.emplace_back((box){991, (int)this->_height - 60, 150, 40},
			0x8DB600, 0xA8D41F, 0x8DB600, 0x8DB600,
			Text("Launch", 1.f, 0x12170D, 0x12170D), true,
			[&engine]()
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
		Text("Selected model :", 1.f, 0x9AA88A, 0x9AA88A),
		std::array<int, 2>{360, 115}));

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
			|| ext != ".mdl" || isInVec(filename, this->_files) != -1)
			continue ;
		int y = 135 + (int)this->_files.size() * 50;
		if (this->_files.size())
		{
			box ldims = this->_files.back().getDimentions();
			y = ldims.y + ldims.h + 10;
		}
		std::string path = dirEntry.path();
		box	pos = {40, y, 305, 40};
		int color = 0x283322;
		int hoverColor = 0x2F3C28;
		int borderColor = 0x4a5a3a;
		int hoverBorderColor = 0x6B8E00;
		Text txt(filename, 1.f, 0xE6EDD8, 0xC5E063);
		bool active = true;
		uintmax_t fileSize = dirEntry.file_size();
		std::function<void()> function = [this, path, filename, fileSize]()
												{
													try
													{
														Agent a(path, filename);
														this->_selectedFile = std::make_shared<Agent>(a);
														this->_selectedFileSize = fileSize;
													}
													catch(const std::exception& e)
													{
														this->_selectedFile = std::make_shared<Agent>(0, 0, 0, filename);
														std::cerr << e.what() << '\n';
													}
													
												};
		this->_files.emplace_back(pos, color, hoverColor, borderColor,
			hoverBorderColor, txt, active, function);
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

		if (std::filesystem::is_regular_file(dirEntry) && ext == ".mdl")
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

void	SelectScreen::onMouseDown(int button, int x, int y)
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
				but.click();
				break ;
			}
		}
		for (Button &file : this->_files)
		{
			if (file.isMouseOn())
			{
				file.click();
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

void	SelectScreen::onMouseUp(int button, int x, int y)
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
	img.putVLine(355, 125, 720, 0x3A4A2E, 3);

	for (Button &but : this->_buttons)
		but.render(img);
	
	renderScrollList(img);

	if (this->_selectedFile)
	{
		Text txt(this->_selectedFile->getName(), 1.f, 0xC8E65A, 0xC8E65A);
		txt.render(363, 145, img, Text::HAlign::Left, Text::VAlign::Center);
		img.putHLine(160, 355, 681, 0x3A4A2C, 3);
		if (!this->_selectedFile->getGamePlayed())
		{
			Text err("Invalid model file", 1.5f, 0xD9644A, 0xD9644A);
			err.render(508, 205, img, Text::HAlign::Center, Text::VAlign::Center);
			return ;
		}
		Rectangle rTrain((box){363, 191, 308, 80}, 0x205930, 2, 0x3A4A2C);
		rTrain.render(img);
		Text train("Training", 1.2f, 0xA6D98A, 0xA6D98A);
		Text session("Sessions", 0.9f, 0xD0D8C4, 0xD0D8C4);
		Text nbSessions(std::to_string(this->_selectedFile->getGamePlayed()), 0.9f, 0xE8F0D8, 0xE8F0D8);
		train.render(368, 201, img, Text::HAlign::Left, Text::VAlign::Top);
		session.render(370, 251, img, Text::HAlign::Left, Text::VAlign::Center);
		nbSessions.render(662, 251, img, Text::HAlign::Right, Text::VAlign::Center);

		Rectangle rParam((box){363, 301, 308, 120}, 0x205930, 2, 0x3A4A2C);
		rParam.render(img);
		Text param("HYPERPARAMETERS", 1.2f, 0xA6D98A, 0xA6D98A);
		Text tlrRate("Learning rate", 0.9f, 0xD0D8C4,0xD0D8C4);
		Text tdiscountF("Discount factor", 0.9f, 0xD0D8C4,0xD0D8C4);
		Text tepsilon("Epsilon", 0.9f, 0xD0D8C4,0xD0D8C4);
		Text lrRate(std::format("{:.2f}", this->_selectedFile->getLrRate()), 0.9f, 0xD0D8C4,0xD0D8C4);
		Text discountF(std::format("{:.2f}", this->_selectedFile->getDiscFacor()), 0.9f, 0xD0D8C4,0xD0D8C4);
		Text epsilon(std::format("{:.2f}", this->_selectedFile->getEpsilon()), 0.9f, 0xD0D8C4,0xD0D8C4);
		param.render(368, 311, img, Text::HAlign::Left, Text::VAlign::Top);
		tlrRate.render(370, 361, img, Text::HAlign::Left, Text::VAlign::Center);
		lrRate.render(662, 361, img, Text::HAlign::Right, Text::VAlign::Center);
		tdiscountF.render(370, 381, img, Text::HAlign::Left, Text::VAlign::Center);
		discountF.render(662, 381, img, Text::HAlign::Right, Text::VAlign::Center);
		tepsilon.render(370, 401, img, Text::HAlign::Left, Text::VAlign::Center);
		epsilon.render(662, 401, img, Text::HAlign::Right, Text::VAlign::Center);

		Rectangle rQTable((box){363, 451, 308, 120}, 0x205930, 2, 0x3A4A2C);
		rQTable.render(img);
		Text Qtbl("Q-Table", 1.2f, 0xA6D98A, 0xA6D98A);
		Text tQtableSize("States", 0.9f, 0xD0D8C4, 0xD0D8C4);
		Text tN0Entries("Non-zero entries", 0.9f, 0xD0D8C4, 0xD0D8C4);
		Text tFSize("File size", 0.9f, 0xD0D8C4, 0xD0D8C4);
		Text QtableSize(std::to_string(this->_selectedFile->getQTableSize()), 0.9f, 0xD0D8C4, 0xD0D8C4);
		Text N0Entries(std::to_string(this->_selectedFile->getQTableN0Entries()), 0.9f, 0xD0D8C4, 0xD0D8C4);
		Text FSize(std::format("{:.2f}", this->_selectedFileSize / 1024.0) + " KB", 0.9f, 0xD0D8C4, 0xD0D8C4);

		Qtbl.render(368, 461, img, Text::HAlign::Left, Text::VAlign::Top);
		tQtableSize.render(370, 511, img, Text::HAlign::Left, Text::VAlign::Center);
		QtableSize.render(662, 511, img, Text::HAlign::Right, Text::VAlign::Center);
		tN0Entries.render(370, 531, img, Text::HAlign::Left, Text::VAlign::Center);
		N0Entries.render(662, 531, img, Text::HAlign::Right, Text::VAlign::Center);
		tFSize.render(370, 551, img, Text::HAlign::Left, Text::VAlign::Center);
		FSize.render(661, 551, img, Text::HAlign::Right, Text::VAlign::Center);
	}
}