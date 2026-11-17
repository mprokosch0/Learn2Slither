#include "learn2Slither.hpp"

SelectScreen::SelectScreen(size_t width, size_t height, Engine &engine): IScreen(width, height),
	_selectedFile((selectedFile){nullptr, textCo(), textCo(), textCo(), std::vector<Rectangle>(),
			Button((box){368, 611, 301, 69}, 0x1F281A, 0x2F3C28, 0x8DB600, 0xA8D41F,
			0x12170D, 0x6B8E00, Text("Use model", 1.f, 0xC5E063, 0xF2F7E6), false,
			[this]()
			{
				this->_settings.model = this->_selectedFile.file;
				this->_settings.useModel = true;
				this->_settings.exportInput.setText(this->_settings.model->getName() + ".mdl");
				this->_settings.onOffImport[0].setPressed(false);
				this->_settings.onOffImport[1].activate();
				this->_settings.onOffImport[1].setPressed(true);
				this->_settings.onOffImport[1].setText(this->_selectedFile.file->getName());
				this->_settings.hyperParametersValues[0].first.setText(std::format("{:.2f}", this->_settings.model->getLrRate()));
				this->_settings.hyperParametersValues[1].first.setText(std::format("{:.2f}", this->_settings.model->getDiscFacor()));
				this->_settings.hyperParametersValues[2].first.setText(std::format("{:.2f}", this->_settings.model->getEpsilon()));
				this->_settings.hyperParametersSlider[0].updatePos(this->_settings.model->getLrRate());
				this->_settings.hyperParametersSlider[1].updatePos(this->_settings.model->getDiscFacor());
				this->_settings.hyperParametersSlider[2].updatePos(this->_settings.model->getEpsilon());
				for (size_t i = 0; i < this->_settings.hyperParametersSlider.size(); i++)
					this->_settings.hyperParametersSlider[i].desactivate();
			}
		)}), _settings((settings){nullptr, false, false, TextBox(), 10, Slider<size_t>(),
			10, Slider<size_t>(), textCo(), textCo(), textCo(), std::vector<Rectangle>(),
			std::array<Button, 3>(), std::array<Button, 2>(), std::array<Button, 2>(), std::array<Button, 2>(), true,
			std::array<float, 3>({0.1f, 0.92f, 0.9}), textCo(), std::array<Slider<float>, 3>(),
			std::vector<Button>()})
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
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("-", 0.9f, 0xC5E063, 0xC5E063), true,
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
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("+", 0.9f, 0xC5E063, 0xC5E063), true,
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
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("-", 0.9f, 0xC5E063, 0xC5E063), true,
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
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("+", 0.9f, 0xC5E063, 0xC5E063), true,
		[&sett]()
		{
			if (sett.height < 40)
			{
				sett.height++;
				sett.values[1].first.setText(std::to_string(sett.height));
				sett.hSlider.updatePos(sett.height);
			}
		});

	sett.onOffImport[0] = Button((box){1085, 212, 140, 30}, 0x283322, 0x2F3C28,
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("New model", 0.9f, 0xC5E063, 0xC5E063), true,
		[&sett]()
		{
			sett.useModel = false;
			for (size_t i = 0; i < sett.hyperParametersSlider.size(); i++)
			{
				sett.hyperParametersValues[i].first.setText(std::format("{:.2f}", sett.hPVs[i]));
				sett.hyperParametersSlider[i].activate();
				sett.hyperParametersSlider[i].updatePos(sett.hPVs[i]);
			}
		}
	);
	sett.onOffImport[0].setPressed(true);
	sett.onOffImport[1] = Button((box){1233, 212, 140, 30}, 0x283322, 0x2F3C28,
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("", 0.9f, 0xC5E063, 0xC5E063), false,
		[&sett]()
		{
			sett.useModel = true;
			sett.exportInput.setText(sett.model->getName() + ".mdl");
			sett.onOffImport[1].setText(sett.model->getName());
			sett.hyperParametersValues[0].first.setText(std::format("{:.2f}", sett.model->getLrRate()));
			sett.hyperParametersValues[1].first.setText(std::format("{:.2f}", sett.model->getDiscFacor()));
			sett.hyperParametersValues[2].first.setText(std::format("{:.2f}", sett.model->getEpsilon()));
			sett.hyperParametersSlider[0].updatePos(sett.model->getLrRate());
			sett.hyperParametersSlider[1].updatePos(sett.model->getDiscFacor());
			sett.hyperParametersSlider[2].updatePos(sett.model->getEpsilon());
			for (size_t i = 0; i < sett.hyperParametersSlider.size(); i++)
				sett.hyperParametersSlider[i].desactivate();
		}
	);

	sett.onOffLearning[0] = Button((box){1085, 427, 140, 30}, 0x283322, 0x2F3C28,
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("On", 0.9f, 0xC5E063, 0xC5E063), true,
		[&sett]()
		{
			sett.learning = true;
		}
	);
	sett.onOffLearning[0].setPressed(true);
	sett.onOffLearning[1] = Button((box){1233, 427, 140, 30}, 0x283322, 0x2F3C28,
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("Off", 0.9f, 0xC5E063, 0xC5E063), true,
		[&sett]()
		{
			sett.learning = false;
		}
	);

	sett.onOffExport[0] = Button((box){1085, 497, 140, 30}, 0x283322, 0x2F3C28,
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("On", 0.9f, 0xC5E063, 0xC5E063), true,
		[&sett]()
		{
			sett.exportModel = true;
			sett.exportInput.activate();
		}
	);
	sett.onOffExport[1] = Button((box){1233, 497, 140, 30}, 0x283322, 0x2F3C28,
		0x4a5a3a, 0x6B8E00, 0x2F3C28, 0x6B8E00, Text("Off", 0.9f, 0xC5E063, 0xC5E063), true,
		[&sett]()
		{
			sett.exportModel = false;
			sett.exportInput.desactivate();
		}
	);
	sett.onOffExport[1].setPressed(true);

	sett.exportInput = TextBox(Text("", 0.9, 0xC5E063, 0x9AA88A),
		Rectangle((box){1085, 537, 288, 30}, 0x283322, 2, 0x4a5a3a), 0x283322, 0x2A3622);

	sett.rectangles.emplace_back(
		Rectangle((box){753, 137, 302, 200}, 0x283322, 2, 0x3A4A2E));
	sett.rectangles.emplace_back(
		Rectangle((box){753, 367, 302, 341}, 0x283322, 2, 0x3A4A2E));
	sett.rectangles.emplace_back(
		Rectangle((box){1078, 137, 302, 450}, 0x283322, 2, 0x3A4A2E));
	
	sett.titles.emplace_back(std::make_pair(
		Text("BOARD SIZE", 1.2f, 0xC5E063, 0xC5E063),
		std::array<int, 2>{758, 147}));
	sett.titles.emplace_back(std::make_pair(
		Text("BOARD PREVIEW", 1.2f, 0xC5E063, 0xC5E063),
		std::array<int, 2>{758, 377}));
	sett.titles.emplace_back(std::make_pair(
		Text("GAME PARAMETERS", 1.2f, 0xC5E063, 0xC5E063),
		std::array<int, 2>{1083, 147}));
	
	
	sett.wSlider = Slider<size_t>((box){764, 222, 280, 3}, 0x3A4A2E, 0x2A3622, 0x8DB600, 0xF2F7E6, 0x4a5a3a, 0x6B8E00, 0x5E6B50, 0x5E6B50, 10, 40);
	sett.hSlider = Slider<size_t>((box){764, 272, 280, 3}, 0x3A4A2E, 0x2A3622, 0x8DB600, 0xF2F7E6, 0x4a5a3a, 0x6B8E00, 0x5E6B50, 0x5E6B50, 10, 40);

	sett.hyperParametersSlider[0] = Slider<float>((box){1089, 282, 280, 3}, 0x3A4A2E, 0x2A3622, 0x8DB600, 0xF2F7E6, 0x4a5a3a, 0x6B8E00, 0x5E6B50, 0x5E6B50, 0.001f, 0.999f);
	sett.hyperParametersSlider[1] = Slider<float>((box){1089, 332, 280, 3}, 0x3A4A2E, 0x2A3622, 0x8DB600, 0xF2F7E6, 0x4a5a3a, 0x6B8E00, 0x5E6B50, 0x5E6B50, 0.001f, 0.999f);
	sett.hyperParametersSlider[2] = Slider<float>((box){1089, 382, 280, 3}, 0x3A4A2E, 0x2A3622, 0x8DB600, 0xF2F7E6, 0x4a5a3a, 0x6B8E00, 0x5E6B50, 0x5E6B50, 0.001f, 0.999f);

	for (int i = 0; i < 150; i+=50)
	{
		sett.hyperParametersValues.emplace_back(std::make_pair(
			Text(std::format("{:.2f}", sett.hPVs[i / 50]), 1.1f, 0xD0D8C4, 0x9AA88A),
			std::array<int, 2>{1373, 257 + i}));
		sett.hyperParametersSlider[i / 50].updatePos(sett.hPVs[i / 50]);
	}

	sett.names.emplace_back(std::make_pair(
		Text("Width", 1.1f, 0xD0D8C4, 0xD0D8C4),
		std::array<int, 2>{760, 197}));
	sett.names.emplace_back(std::make_pair(
		Text("Height", 1.1f, 0xD0D8C4, 0xD0D8C4),
		std::array<int, 2>{760, 247}));
	sett.names.emplace_back(std::make_pair(
		Text("Start from", 1.1f, 0xD0D8C4, 0xD0D8C4),
		std::array<int, 2>{1085, 197}));
	sett.names.emplace_back(std::make_pair(
		Text("Learning rate", 1.1f, 0xD0D8C4, 0x9AA88A),
		std::array<int, 2>{1085, 257}));
	sett.names.emplace_back(std::make_pair(
		Text("Discount factor", 1.1f, 0xD0D8C4, 0x9AA88A),
		std::array<int, 2>{1085, 307}));
	sett.names.emplace_back(std::make_pair(
		Text("Exploration rate", 1.1f, 0xD0D8C4, 0x9AA88A),
		std::array<int, 2>{1085, 357}));
	sett.names.emplace_back(std::make_pair(
		Text("Learning", 1.1f, 0xD0D8C4, 0x9AA88A),
		std::array<int, 2>{1085, 407}));
	sett.names.emplace_back(std::make_pair(
		Text("Save model at the end", 1.1f, 0xD0D8C4, 0x9AA88A),
		std::array<int, 2>{1085, 477}));
	


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