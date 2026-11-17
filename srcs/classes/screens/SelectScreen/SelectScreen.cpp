#include "learn2Slither.hpp"

static box		clip(const box &button, const Rectangle &rectangle)
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
	for (Button &but : this->_settings.onOffImport)
	{
		but.clearHover();
		but.hoverIfOnButton(x, y);
	}
	for (Button &but : this->_settings.onOffExport)
	{
		but.clearHover();
		but.hoverIfOnButton(x, y);
	}
	for (Button &but : this->_settings.onOffLearning)
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
	for (Slider<float> &sli : this->_settings.hyperParametersSlider)
	{
		sli.clearHover();
		sli.hoverIfOnSlider(x, y);
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

void	SelectScreen::onKeyDown(int keycode)
{
	if (this->_settings.exportInput.isFocused())
	{
		if (this->_hooks.lShiftKey && std::isalpha(keycode))
			keycode -= 32;
		this->_settings.exportInput.doAction(keycode);
	}
	updateKeyHooks(static_cast<keycodes>(keycode), HOOK_DOWN);
}


void	SelectScreen::onKeyUp(int keycode)
{
	updateKeyHooks(static_cast<keycodes>(keycode), HOOK_UP);
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
		bool onbutton = false;
		for (Button &but : this->_buttons)
		{
			if (but.isMouseOn())
			{
				but.click();
				break ;
			}
		}
		if (this->_settings.onOffImport[0].isMouseOn())
		{
			this->_settings.onOffImport[0].click();
			this->_settings.onOffImport[1].setPressed(false);
		}
		else if (this->_settings.onOffImport[1].isMouseOn())
		{
			this->_settings.onOffImport[1].click();
			this->_settings.onOffImport[0].setPressed(false);
		}
		if (this->_settings.onOffExport[0].isMouseOn())
		{
			this->_settings.onOffExport[0].click();
			this->_settings.onOffExport[1].setPressed(false);
		}
		else if (this->_settings.onOffExport[1].isMouseOn())
		{
			this->_settings.onOffExport[1].click();
			this->_settings.onOffExport[0].setPressed(false);
		}
		if (this->_settings.onOffLearning[0].isMouseOn())
		{
			this->_settings.onOffLearning[0].click();
			this->_settings.onOffLearning[1].setPressed(false);
		}
		else if (this->_settings.onOffLearning[1].isMouseOn())
		{
			this->_settings.onOffLearning[1].click();
			this->_settings.onOffLearning[0].setPressed(false);
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
		for (size_t i = 0; i < this->_settings.hyperParametersSlider.size(); i++)
		{
			Slider<float> &slider = this->_settings.hyperParametersSlider[i];
			if (!slider.isMouseOn())
				continue;
			slider.click(x, this->_settings.hPVs[i]);
			this->_settings.hyperParametersValues[i].first.setText(std::format("{:.2f}", this->_settings.hPVs[i]));
		}
		onbutton = false;
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
		if (this->_settings.exportInput.isMouseOn(x, y))
			this->_settings.exportInput.setfocus(true);
		else
			this->_settings.exportInput.setfocus(false);
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
	updateMouseHooks(button, HOOK_DOWN);
}

void	SelectScreen::onMouseUp(int button, int x, int y)
{
	hoverUpdate(x, y);
	updateMouseHooks(button, HOOK_UP);
}

void	SelectScreen::onMouseMove(int x, int y)
{
	if (!this->_hooks.mouseLeft)
		hoverUpdate(x, y);
	else
	{
		for (size_t i = 0; i < this->_settings.hyperParametersSlider.size(); i++)
		{
			Slider<float> &slider = this->_settings.hyperParametersSlider[i];
			if (!slider.isMouseOn())
				continue;
			slider.click(x, this->_settings.hPVs[i]);
			this->_settings.hyperParametersValues[i].first.setText(std::format("{:.2f}", this->_settings.hPVs[i]));
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
	}
		
}
