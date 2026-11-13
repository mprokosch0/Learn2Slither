#include "learn2Slither.hpp"

SelectScreen::SelectScreen(size_t width, size_t height, Engine &engine): IScreen(width, height),
	_selectedFile((selectedFile){nullptr, textCo(), textCo(), textCo(), std::vector<Rectangle>(),
			Button((box){368, 611, 301, 69}, 0x1F281A, 0x2F3C28, 0x8DB600, 0xA8D41F,
			0x12170D, 0x6B8E00, Text("Use model", 1.f, 0xC5E063, 0xF2F7E6), false,
			[]()
			{}
		)}), _settings((settings){std::string(), false, 10, Slider<size_t>(), 10, Slider<size_t>(),
			textCo(), textCo(), textCo(), std::vector<Rectangle>(),
			std::array<Button, 3>(), std::vector<Button>()})
{
	this->_buttons.emplace_back((box){30, 15, 150, 30}, 0x1A2113, 0x8DB600,
			0x8DB600, 0x8DB600, 0x8DB600, 0x8DB600, Text("Back", 1.f, 0xC5E063, 0x12170D), true,
			[width, height, &engine]()
			{
				engine.prepareNextScreen(std::make_unique<TitleScreen>(width, height, engine));
			});
	this->_buttons.emplace_back((box){991, (int)this->_height - 60, 150, 40},
			0x8DB600, 0xA8D41F, 0x8DB600, 0x8DB600, 0x8DB600, 0x8DB600,
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
	
	initModelSelection();
	initSettings();
}

void	SelectScreen::initSettings(void)
{
	settings &sett = this->_settings;
	
	sett.boardSize[0] = Button((box){760, 297, 89, 30}, 0x283322, 0x2F3C28,
		0x3A4A2E, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("Small", 0.9f, 0xD0D8C4, 0xD0D8C4), true,
		[&sett]()
		{
			sett.width = 10;
			sett.height = 10;
			sett.values[0].first.setText(std::to_string(sett.width));
			sett.values[1].first.setText(std::to_string(sett.height));
			sett.wSlider.updatePos(sett.width);
			sett.hSlider.updatePos(sett.height);
		}
	);

	sett.boardSize[1] = Button((box){859, 297, 89, 30}, 0x283322, 0x2F3C28,
		0x3A4A2E, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("Medium", 0.9f, 0xD0D8C4, 0xD0D8C4), true,
		[&sett]()
		{
			sett.width = 25;
			sett.height = 25;
			sett.values[0].first.setText(std::to_string(sett.width));
			sett.values[1].first.setText(std::to_string(sett.height));
			sett.wSlider.updatePos(sett.width);
			sett.hSlider.updatePos(sett.height);
		}
	);

	sett.boardSize[2] = Button((box){958, 297, 89, 30}, 0x283322, 0x2F3C28,
		0x3A4A2E, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("Large", 0.9f, 0xD0D8C4, 0xD0D8C4), true,
		[&sett]()
		{
			sett.width = 40;
			sett.height = 40;
			sett.values[0].first.setText(std::to_string(sett.width));
			sett.values[1].first.setText(std::to_string(sett.height));
			sett.wSlider.updatePos(sett.width);
			sett.hSlider.updatePos(sett.height);
		}
	);

	sett.buttons.emplace_back((box){830, 188, 20, 20}, 0x283322, 0x2F3C28,
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("-", 0.9f, 0xffffff, 0xffffff), true,
		[&sett]()
		{
			if (sett.width > 10)
			{
				sett.width--;
				sett.values[0].first.setText(std::to_string(sett.width));
				sett.wSlider.updatePos(sett.width);
			}
		});
	sett.buttons.emplace_back((box){900, 188, 20, 20}, 0x283322, 0x2F3C28,
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("+", 0.9f, 0xffffff, 0xffffff), true,
		[&sett]()
		{
			if (sett.width < 40)
			{
				sett.width++;
				sett.values[0].first.setText(std::to_string(sett.width));
				sett.wSlider.updatePos(sett.width);
			}
		});
	sett.buttons.emplace_back((box){830, 238, 20, 20}, 0x283322, 0x2F3C28,
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("-", 0.9f, 0xffffff, 0xffffff), true,
		[&sett]()
		{
			if (sett.height > 10)
			{
				sett.height--;
				sett.values[1].first.setText(std::to_string(sett.height));
				sett.hSlider.updatePos(sett.height);
			}
		});
	sett.buttons.emplace_back((box){900, 238, 20, 20}, 0x283322, 0x2F3C28,
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("+", 0.9f, 0xffffff, 0xffffff), true,
		[&sett]()
		{
			if (sett.height < 40)
			{
				sett.height++;
				sett.values[1].first.setText(std::to_string(sett.height));
				sett.hSlider.updatePos(sett.height);
			}
		});

	sett.rectangles.emplace_back(
		Rectangle((box){753, 137, 302, 200}, 0x283322, 2, 0x3A4A2E));
	
	sett.rectangles.emplace_back(
		Rectangle((box){753, 367, 302, 341}, 0x283322, 2, 0x3A4A2E));
	
	sett.titles.emplace_back(std::make_pair(
		Text("BOARD SIZE", 1.2f, 0xC5E063, 0xC5E063),
		std::array<int, 2>{758, 147}));
	
	sett.titles.emplace_back(std::make_pair(
		Text("BOARD PREVIEW", 1.2f, 0xC5E063, 0xC5E063),
		std::array<int, 2>{758, 377}));
	
	
	sett.wSlider = Slider<size_t>((box){764, 222, 280, 3}, 0x8DB600, 0xF2F7E6, 0x4a5a3a, 0x6B8E00, 10, 40);
	sett.hSlider = Slider<size_t>((box){764, 272, 280, 3}, 0x8DB600, 0xF2F7E6, 0x4a5a3a, 0x6B8E00, 10, 40);


	sett.names.emplace_back(std::make_pair(
		Text("Width", 1.1f, 0xD0D8C4, 0xD0D8C4),
		std::array<int, 2>{760, 197}));
	
	sett.names.emplace_back(std::make_pair(
		Text("Height", 1.1f, 0xD0D8C4, 0xD0D8C4),
		std::array<int, 2>{760, 247}));
	


	sett.values.emplace_back(std::make_pair(
		Text(std::to_string(sett.width), 1.f, 0xD0D8C4, 0xD0D8C4),
		std::array<int, 2>{875, 197}));
	
	sett.values.emplace_back(std::make_pair(
		Text(std::to_string(sett.width), 1.f, 0xD0D8C4, 0xD0D8C4),
		std::array<int, 2>{875, 247}));
}

void	SelectScreen::initModelSelection(void)
{
	this->_selectedFile.titles.emplace_back(std::make_pair(
		Text("TRAINING", 1.2f, 0xC5E063, 0xC5E063),
		std::array<int, 2>{368, 201}));

	this->_selectedFile.titles.emplace_back(std::make_pair(
		Text("HYPERPARAMETERS", 1.2f, 0xC5E063, 0xC5E063),
		std::array<int, 2>{368, 311}));

	this->_selectedFile.titles.emplace_back(std::make_pair(
		Text("Q-Table", 1.2f, 0xC5E063, 0xC5E063),
		std::array<int, 2>{368, 461}));


	this->_selectedFile.names.emplace_back(std::make_pair(
		Text("", 1.f, 0xC8E65A, 0xC8E65A),
		std::array<int, 2>{363, 145}));
	
	this->_selectedFile.names.emplace_back(std::make_pair(
		Text("Sessions", 0.9f, 0xD0D8C4, 0xD0D8C4),
		std::array<int, 2>{370, 251}));
	
	this->_selectedFile.names.emplace_back(std::make_pair(
		Text("Learning rate", 0.9f, 0xD0D8C4,0xD0D8C4),
		std::array<int, 2>{370, 361}));
	
	this->_selectedFile.names.emplace_back(std::make_pair(
		Text("Discount factor", 0.9f, 0xD0D8C4,0xD0D8C4),
		std::array<int, 2>{370, 381}));
	
	this->_selectedFile.names.emplace_back(std::make_pair(
		Text("Epsilon", 0.9f, 0xD0D8C4,0xD0D8C4),
		std::array<int, 2>{370, 401}));
	
	this->_selectedFile.names.emplace_back(std::make_pair(
		Text("States", 0.9f, 0xD0D8C4, 0xD0D8C4),
		std::array<int, 2>{370, 511}));
	
	this->_selectedFile.names.emplace_back(std::make_pair(
		Text("Non-zero entries", 0.9f, 0xD0D8C4, 0xD0D8C4),
		std::array<int, 2>{370, 531}));
	
	this->_selectedFile.names.emplace_back(std::make_pair(
		Text("File size", 0.9f, 0xD0D8C4, 0xD0D8C4),
		std::array<int, 2>{370, 551}));
	

	this->_selectedFile.values.emplace_back(std::make_pair(
		Text("", 0.9f, 0xE8F0D8, 0xE8F0D8),
		std::array<int, 2>{664, 251}));
	
	this->_selectedFile.values.emplace_back(std::make_pair(
		Text("", 0.9f, 0xE8F0D8, 0xE8F0D8),
		std::array<int, 2>{664, 361}));
	
	this->_selectedFile.values.emplace_back(std::make_pair(
		Text("", 0.9f, 0xE8F0D8, 0xE8F0D8),
		std::array<int, 2>{664, 381}));
	
	this->_selectedFile.values.emplace_back(std::make_pair(
		Text("", 0.9f, 0xE8F0D8, 0xE8F0D8),
		std::array<int, 2>{664, 401}));
	
	this->_selectedFile.values.emplace_back(std::make_pair(
		Text("", 0.9f, 0xE8F0D8, 0xE8F0D8),
		std::array<int, 2>{664, 511}));
	
	this->_selectedFile.values.emplace_back(std::make_pair(
		Text("", 0.9f, 0xE8F0D8, 0xE8F0D8),
		std::array<int, 2>{664, 531}));
	
	this->_selectedFile.values.emplace_back(std::make_pair(
		Text("", 0.9f, 0xE8F0D8, 0xE8F0D8),
		std::array<int, 2>{663, 551}));
	
	
	this->_selectedFile.rectangles.emplace_back(
		Rectangle((box){363, 191, 311, 80}, 0x283322, 2, 0x3A4A2E));
	this->_selectedFile.rectangles.emplace_back(
		Rectangle((box){363, 301, 311, 120}, 0x283322, 2, 0x3A4A2E));
	this->_selectedFile.rectangles.emplace_back(
		Rectangle((box){363, 451, 311, 120}, 0x283322, 2, 0x3A4A2E));
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
		int pressedColor = 0x2F3C28;
		int pressedBorderColor = 0x6B8E00;
		Text txt(filename, 1.f, 0xE6EDD8, 0xC5E063);
		bool active = true;
		uintmax_t fileSize = dirEntry.file_size();
		std::function<void()> function =
			[this, path, filename, fileSize]()
			{
				this->_selectedFile.useModel.setPressed(false);
				try
				{
					Agent a(path, filename);
					this->_selectedFile.file = std::make_shared<Agent>(a);
					this->_selectedFile.names[0].first.setText(a.getName());
					this->_selectedFile.values[0].first.setText(std::to_string(a.getGamePlayed()));
					this->_selectedFile.values[1].first.setText(std::format("{:.2f}", a.getLrRate()));
					this->_selectedFile.values[2].first.setText(std::format("{:.2f}", a.getDiscFacor()));
					this->_selectedFile.values[3].first.setText(std::format("{:.2f}", a.getEpsilon()));
					this->_selectedFile.values[4].first.setText(std::to_string(a.getQTableSize()));
					this->_selectedFile.values[5].first.setText(std::to_string(a.getQTableN0Entries()));
					this->_selectedFile.values[6].first.setText(std::format("{:.2f}", fileSize / 1024.0) + " KB");
					this->_selectedFile.useModel.activate();
				}
				catch(const std::exception& e)
				{
					this->_selectedFile.file = std::make_shared<Agent>(0, 0, 0, filename);
					this->_selectedFile.useModel.desactivate();
					std::cerr << e.what() << '\n';
				}
				
			};
		this->_files.emplace_back(pos, color, hoverColor, borderColor,
			hoverBorderColor, pressedColor, pressedBorderColor, txt, active, function);
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

void	SelectScreen::hoverUpdate(int x, int y)
{
	for (Button &but : this->_buttons)
	{
		but.clearHover();
		but.hoverIfOnButton(x, y);
	} 
	for (Button &but : this->_settings.buttons)
	{
		but.clearHover();
		but.hoverIfOnButton(x, y);
	}
	for (Button &but : this->_settings.boardSize)
	{
		but.clearHover();
		but.hoverIfOnButton(x, y);
	}
	this->_settings.wSlider.clearHover();
	this->_settings.wSlider.hoverIfOnSlider(x, y);
	this->_settings.hSlider.clearHover();
	this->_settings.hSlider.hoverIfOnSlider(x, y);
	for (Button &file : this->_files)
	{
		file.clearHover();
		box cliped = clip(file.getDimentions(), this->_fileListRect);
		file.hoverIfOnButtonClip(x, y, cliped);
	}
	if (this->_selectedFile.file)
	{
		this->_selectedFile.useModel.clearHover();
		this->_selectedFile.useModel.hoverIfOnButton(x, y);
	}
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
	hoverUpdate(x, y);
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
		for (Button &but : this->_settings.buttons)
		{
			if (but.isMouseOn())
			{
				but.click();
				but.setPressed(false);
				break ;
			}
		}
		for (Button &but : this->_settings.boardSize)
		{
			if (but.isMouseOn())
			{
				but.click();
				but.setPressed(false);
				break ;
			}
		}

		if (this->_settings.wSlider.isMouseOn())
		{
			this->_settings.wSlider.click(x, this->_settings.width);
			this->_settings.values[0].first.setText(std::to_string(this->_settings.width));
		}
		if (this->_settings.hSlider.isMouseOn())
		{
			this->_settings.hSlider.click(x, this->_settings.height);
			this->_settings.values[1].first.setText(std::to_string(this->_settings.height));
		}
		
		bool onbutton = false;
		for (Button &file : this->_files)
		{
			if (file.isMouseOn())
			{
				file.click();
				onbutton = true;
				break ;
			}
		}
		if (onbutton)
		{
			for (Button &file : this->_files)
			{
				if (!file.isMouseOn())
					file.setPressed(false);
			}
		}
		if (this->_selectedFile.file && this->_selectedFile.useModel.isMouseOn())
			this->_selectedFile.useModel.click();
	}
	if (button == M_SCROLL_DOWN || button == M_SCROLL_UP)
	{
		box rectDims = this->_fileListRect.getDimentions();
		if (x >= rectDims.x && x < rectDims.x + rectDims.w / 2
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
	hoverUpdate(x, y);
	updateHooks(button, HOOK_UP);
}

void	SelectScreen::onMouseMove(int x, int y)
{
	if (!this->_hooks.mouseLeft)
		hoverUpdate(x, y);
	else
	{
		if (this->_settings.wSlider.isMouseOn())
		{
			this->_settings.wSlider.click(x, this->_settings.width);
			this->_settings.values[0].first.setText(std::to_string(this->_settings.width));
		}
		if (this->_settings.hSlider.isMouseOn())
		{
			this->_settings.hSlider.click(x, this->_settings.height);
			this->_settings.values[1].first.setText(std::to_string(this->_settings.height));
		}
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

void	SelectScreen::renderSelectedFile(Image &img) const
{
	Text txt(this->_selectedFile.file->getName(), 1.f, 0xC8E65A, 0xC8E65A);
	txt.render(363, 145, img, Text::HAlign::Left, Text::VAlign::Center);
	img.putHLine(160, 355, 681, 0x3A4A2C, 3);
	if (!this->_selectedFile.file->getGamePlayed())
	{
		Text err("Invalid model file", 1.5f, 0xD9644A, 0xD9644A);
		err.render(508, 205, img, Text::HAlign::Center, Text::VAlign::Center);
		return ;
	}

	for (const Rectangle &r : this->_selectedFile.rectangles)
		r.render(img);
	
	for (const std::pair<Text, std::array<int, 2>> &t : this->_selectedFile.titles)
		t.first.render(t.second[0], t.second[1], img, Text::HAlign::Left, Text::VAlign::Top);
	
	for (const std::pair<Text, std::array<int, 2>> &t : this->_selectedFile.names)
		t.first.render(t.second[0], t.second[1], img, Text::HAlign::Left, Text::VAlign::Center);
	
	for (const std::pair<Text, std::array<int, 2>> &t : this->_selectedFile.values)
		t.first.render(t.second[0], t.second[1], img, Text::HAlign::Right, Text::VAlign::Center);
	
	this->_selectedFile.useModel.render(img);
}

void	SelectScreen::renderSettings(Image &img) const
{
	const settings &sett = this->_settings;
	for (const Rectangle &r : sett.rectangles)
		r.render(img);

	for (const std::pair<Text, std::array<int, 2>> &t : sett.titles)
		t.first.render(t.second[0], t.second[1], img, Text::HAlign::Left, Text::VAlign::Top);
	
	for (const std::pair<Text, std::array<int, 2>> &t : sett.names)
		t.first.render(t.second[0], t.second[1], img, Text::HAlign::Left, Text::VAlign::Center);
	
	for (const std::pair<Text, std::array<int, 2>> &t : sett.values)
		t.first.render(t.second[0], t.second[1], img, Text::HAlign::Center, Text::VAlign::Center);

	for (const Button &b : sett.buttons)
		b.render(img);
	
	for (const Button &b : sett.boardSize)
		b.render(img);

	sett.wSlider.render(img);
	sett.hSlider.render(img);
}

void	SelectScreen::renderPreview(Image &img) const
{
	int width = static_cast<int>(this->_settings.width) * 7;
	int height = static_cast<int>(this->_settings.height) * 7;
	box board =
	{
		.x = 904 - width / 2,
		.y = 552 - height / 2,
		.w = width,
		.h = height
	};

	img.putRectangleBorder(board, 0x8DB600, 1, 0x3A4A2E);

	for (int i = board.x + 7; i < board.x + width; i += 7)
		img.putVLine(i, board.y, board.y + board.h, 0x3A4A2E, 1);
	
	for (int j = board.y + 7; j < board.y + height; j += 7)
		img.putHLine(j, board.x, board.x + board.w, 0x3A4A2E, 1);
}

void	SelectScreen::render(Image &img)
{
	img.colorImage(0x12170D);
	this->_fileListRect.render(img);
	for (Rectangle &rect : this->_rectangles)
		rect.render(img);
	
	this->_title.first.render(this->_title.second[0], this->_title.second[1], img,
		Text::HAlign::Center, Text::VAlign::Center);

	for (const std::pair<Text, std::array<int, 2>> &pair : this->_texts)
		pair.first.render(pair.second[0], pair.second[1], img);

	img.putVLine(this->_width / 2, 60, this->_height, 0x3A4A2E, 3);
	img.putVLine(1066, 125, 720, 0x3A4A2E, 3);
	img.putVLine(355, 125, 720, 0x3A4A2E, 3);

	for (Button &but : this->_buttons)
		but.render(img);
	
	renderScrollList(img);
	renderSettings(img);
	renderPreview(img);
	if (this->_selectedFile.file)
		renderSelectedFile(img);
}