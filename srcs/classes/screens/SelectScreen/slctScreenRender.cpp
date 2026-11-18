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

void	SelectScreen::renderScrollList(Image &img)
{
	for (Button &but : this->_files)
	{
		box butDims = but.getDimentions();
		box clipedBox = clip(butDims, this->_fileListRect);
		if (clipedBox.w == 0 || clipedBox.h == 0)
			continue ;
		
		but.renderClip(img, clipedBox);
	}
}

void	SelectScreen::renderSelectedFile(Image &img)
{
	Text txt(this->_selectedFile.file->getName(), 1.f, 0xC8E65A, 0xC8E65A);
	box clip = {355, 125, 323, 35};
	txt.renderClip(363, 145, img, clip, Text::HAlign::Left, Text::VAlign::Center);
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

void	SelectScreen::renderSettings(Image &img)
{
	settings &sett = this->_settings;
	for (const Rectangle &r : sett.rectangles)
		r.render(img);

	for (const std::pair<Text, std::array<int, 2>> &t : sett.titles)
		t.first.render(t.second[0], t.second[1], img, Text::HAlign::Left, Text::VAlign::Top);
	
	for (const std::pair<Text, std::array<int, 2>> &t : sett.names)
		t.first.render(t.second[0], t.second[1], img, Text::HAlign::Left, Text::VAlign::Center);
	
	for (const std::pair<Text, std::array<int, 2>> &t : sett.values)
		t.first.render(t.second[0], t.second[1], img, Text::HAlign::Center, Text::VAlign::Center);

	for (const std::pair<Text, std::array<int, 2>> &t : sett.hyperParametersValues)
	{
		if (!sett.useModel)
			t.first.render(t.second[0], t.second[1], img, Text::HAlign::Right, Text::VAlign::Center);
		else
			t.first.render(t.second[0], t.second[1], img, Text::HAlign::Right, Text::VAlign::Center, Text::Colors::Secondary);
	}

	for (Button &b : sett.buttons)
		b.render(img);
	
	for (Button &b : sett.boardSize)
		b.render(img);

	for (Button &b : sett.onOffImport)
		b.render(img);

	for (Button &b : sett.onOffExport)
		b.render(img);
	
	for (Button &b : sett.onOffLearning)
		b.render(img);
	
	for (Button &b : sett.onOffDisplay)
		b.render(img);
	
	for (Slider<double> &s : sett.hyperParametersSlider)
		s.render(img);
	
	sett.nbGamesBox.render(img);

	sett.exportInput.render(img);

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