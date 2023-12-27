#include <SFML/Graphics.hpp>
#include "gameMenu.h"
#include "singlepalyer.h"
#include <iostream>
#include <string>
#include "settings.h"
#include "twoPlayaGame.h"

/**
*   Инициализирует название игры в меню.
* 
*   @param sf::Text& mtext           Результирующий текст.
*   @param float xpos                X-овая позиция текста.
*   @param float ypos                Y-овая позиция текста.
*   @param sf::String str            Содержимое текста.
*   @param int size_font             Размер шрифта.
*   @param sf::Color menu_text_color Цвет текста.
*   @param int bord                  Толщина контура.
*   @param sf::Color border_color    Цвет контура.
*
*   @return void
*/
void InitText(sf::Text& mtext, float xpos, float ypos, sf::String str, int size_font = 60,
	sf::Color menu_text_color = sf::Color::Black, int bord = 0, sf::Color border_color = sf::Color::White);

void setControl(sf::Text& key, sf::String keyName, float ypos);

int gameSingleplayerStart(sf::RenderWindow& window, singleplayer game);

int main()
{
	// Создаем окно игры.
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SNAKES", sf::Style::Fullscreen);

	window.setFramerateLimit(60);

	std::vector<std::pair<std::string, std::string>> settings;

	try {
		settings = getSettings();
	}
	catch (...) {
		settings = setDefaultSettings();
	}

	// Делаем курсор невидимым.
	window.setMouseCursorVisible(false);

	// Размеры окна
	float width = sf::VideoMode::getDesktopMode().width;
	float height = sf::VideoMode::getDesktopMode().height;

	// Устанавливает задний фон меню.
	sf::RectangleShape background(sf::Vector2f(width, height));
	sf::Texture texture_window;
	if (!texture_window.loadFromFile("../designe/background_menu2.jpg")) return 4;
	background.setTexture(&texture_window);

	// Устанавливает шрифт.
	sf::Font font;
	if (!font.loadFromFile("../designe/font/menuFont.ttf")) return 5;

	// Устанавливает заголовок в главном меню.
	sf::Text titul;
	titul.setFont(font);
	InitText(titul, 580, 50, "SNAKES", 150, sf::Color(40, 40, 40), 3, sf::Color::Yellow);

	// Устанавливает заголовок в главном меню.
	sf::Text developers;
	developers.setFont(font);
	InitText(developers, 580, 350, "DEVELOPERS:", 90, sf::Color(40, 40, 40), 3, sf::Color::Yellow);

	// Устанавливает заголовок в главном меню.
	sf::Text owner;
	owner.setFont(font);
	InitText(owner, 580, 450, "AVERCHENKO N.A.", 90, sf::Color(40, 40, 40), 3, sf::Color::Yellow);

	// Устанавливает заголовок в главном меню.
	sf::Text developer;
	developer.setFont(font);
	InitText(developer, 580, 550, "PETUKHOV D.A.", 90, sf::Color(40, 40, 40), 3, sf::Color::Yellow);

	// Устанавливает заголовок в главном меню.
	sf::Text kanye;
	kanye.setFont(font);
	InitText(kanye, 580, 650, "BELIKOV V.V.", 90, sf::Color(40, 40, 40), 3, sf::Color::Yellow);

	// Инициализирует главное меню.
	std::vector<sf::String> name = { "START", "SETTINGS", "ABOUT", "EXIT" };
	gameMenu gameMenu_(window, 950, 400, name, 130, 100);
	gameMenu_.alignTextMenu(1);

	// Инициализирует текст вложенных меню
	std::vector<sf::String> startGameName = { "SINGLEPLAYER", "MULTIPLAYER", "GO BACK" };
	std::vector<sf::String> settingsGameName = { "NICKNAME", "CONTROL", "THEME", "FIELD SIZE", "GO BACK" };
	std::vector<sf::String> aboutGameName = { "GO BACK" };

	// Инициализирует меню управления.
	std::vector<sf::String> controlName = { "UP", "LEFT", "DOWN", "RIGHT", "APPLY", "GO BACK" };
	gameMenu controlMenu(window, 950, 350, controlName, 100, 70);
	controlMenu.alignTextMenu(2);

	// Инициализирует меню одиночной игры.
	std::vector<sf::String> singleplayerName = { "ROUNDS", "BOTS", "PLAY", "GO BACK" };
	gameMenu singleplayerMenu(window, 950, 350, singleplayerName, 100, 70);
	singleplayerMenu.alignTextMenu(2);

	// Инициализирует меню одиночной игры.
	std::vector<sf::String> multyplayerName = { "ROUNDS", "PLAY", "GO BACK" };
	gameMenu multyplayerMenu(window, 950, 450, multyplayerName, 100, 70);
	multyplayerMenu.alignTextMenu(2);

	// Инициализирует меню изменения имени игрока.
	std::vector<sf::String> NicknamesName = { "PLAYER1", "PLAYER2", "SAVE", "GO BACK" };
	gameMenu NicknamesMenu(window, 950, 350, NicknamesName, 100, 70);
	NicknamesMenu.alignTextMenu(2);

	// Инициализирует меню изменения размера поля.
	std::vector<sf::String> fieldName = { "BIG", "MEDIUM", "SMALL", "GO BACK" };
	gameMenu fieldMenu(window, 950, 350, fieldName, 100, 70);
	fieldMenu.alignTextMenu(1);

	// Инициализирует меню изменения имени игрока.
	std::vector<sf::String> themsName = { "PLAYER1", "PLAYER2", "APPLE", "BOT", "SAVE", "GO BACK" };
	gameMenu themsMenu(window, 950, 350, themsName, 100, 70);
	themsMenu.alignTextMenu(2);

	// Инициализирует меню выхода игрока.
	std::vector<sf::String> exitGameName = { "GO BACK", "EXIT" };

	sf::Text exit1;
	exit1.setFont(font);
	InitText(exit1, 380, 280, "DO YOU WANT TO", 100, sf::Color(40, 40, 40), 3, sf::Color::Yellow);
	sf::Text exit2;
	exit2.setFont(font);
	InitText(exit2, 380, 400, "LEAVE THE GAME?", 100, sf::Color(40, 40, 40), 3, sf::Color::Yellow);

	sf::Text nickname1;
	sf::Text nickname2;
	sf::Text keyUp;
	sf::Text keyLeft;
	sf::Text keyDown;
	sf::Text keyRight;

	sf::Text keyRoundsCount;
	sf::Text keyBot;

	sf::String keyName;

	bool bot = false;

	std::vector<std::string> botSettings;
	botSettings.push_back("OFF");
	botSettings.push_back("ON");
	auto botIsSetIt = std::find(botSettings.begin(), botSettings.end(), settings[5].second);


	std::vector<std::string> playerColorSettings;
	playerColorSettings.push_back("1");
	playerColorSettings.push_back("2");
	playerColorSettings.push_back("3");
	playerColorSettings.push_back("4");
	playerColorSettings.push_back("5");
	playerColorSettings.push_back("6");
	playerColorSettings.push_back("7");
	playerColorSettings.push_back("8");
	playerColorSettings.push_back("9");

	std::vector<std::string> appleColorSettings;
	appleColorSettings.push_back("1");
	appleColorSettings.push_back("2");
	appleColorSettings.push_back("3");
	appleColorSettings.push_back("4");

	std::vector<std::string> botColorSettings;
	botColorSettings.push_back("1");
	botColorSettings.push_back("2");
	botColorSettings.push_back("3");
	botColorSettings.push_back("4");

	bool isSettings = false;
	bool about = false;
	auto botColor = settings[12].second;
	auto appleColor = settings[11].second;
	auto player2Color = settings[10].second;
	auto player1Color = settings[9].second;

	// Отрисовывает окно.
	while (window.isOpen())
	{
		sf::Event event;

		twoPlayaGame multyGame(settings[6].second, settings[7].second);

		// Обрабатывает события.
		while (window.pollEvent(event))
		{
			// Закрывает окно.
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
				window.close();

			// Обрабатывает нажатие кнопки.
			if (event.type == sf::Event::KeyReleased) {

				// Выбор нижестоящей кнопки.
				if (event.key.code == sf::Keyboard::Up) {
					gameMenu_.moveUp();
				}

				// Выбор нижестоящей кнопки.
				if (event.key.code == sf::Keyboard::Down) {
					gameMenu_.moveDown();
				}


				switch (gameMenu_.getMode()) {

					// Главное меню.
				case 0:

					// Обрабатывает нажатие кнопки.
					if (event.key.code != sf::Keyboard::Enter) {
						break;
					}

					switch (gameMenu_.getSelected()) {
					case 0:
						gameMenu_.setPositionY(450);
						gameMenu_.pressButton(startGameName, 1);
						break;
					case 1:
						gameMenu_.setPositionY(350);
						gameMenu_.pressButton(settingsGameName, 2);
						break;
					case 2:
						about = true;
						gameMenu_.setPositionY(750);
						gameMenu_.pressButton(aboutGameName, 3);
						break;
					case 3:
						gameMenu_.setPositionY(550);
						gameMenu_.pressButton(exitGameName, 4);
						break;
					}

					break;

					// Меню начала игры.
				case 1: {

					// Обрабатывает нажатие кнопки.
					if (event.key.code != sf::Keyboard::Enter) {
						break;
					}

					switch (gameMenu_.getSelected()) {
					case 0: {

						bool singleplayerIsStaart = true;

						// Обрабатывает нажатие кнопки.
						sf::String roundsCount = settings[4].second;
						sf::String botIsSet = settings[5].second;

						while (singleplayerIsStaart) {

							sf::Keyboard::Scancode code = sf::Keyboard::Scancode::Unknown;

							while (window.pollEvent(event))
							{
								// Закрывает окно.
								if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
									window.close();

								// Обрабатывает нажатие кнопки.
								if (event.type == sf::Event::KeyReleased) {

									// Выбор нижестоящей кнопки.
									if (event.key.code == sf::Keyboard::Up) {
										singleplayerMenu.moveUp();
									}

									// Выбор нижестоящей кнопки.
									if (event.key.code == sf::Keyboard::Down) {
										singleplayerMenu.moveDown();
									}

									switch (singleplayerMenu.getSelected()) {

									case 0:
										if (event.type == sf::Event::KeyReleased) {
											code = event.key.scancode;
											if (sf::Keyboard::getDescription(code) >= '1' && sf::Keyboard::getDescription(code) <= '9') {
												roundsCount = sf::Keyboard::getDescription(code);
											}
										}

										break;
									case 1:
										if (event.type == sf::Event::KeyReleased) {
											code = event.key.scancode;
											if (sf::Keyboard::getDescription(code) == "Left") {

												if (botIsSetIt != botSettings.begin()) {
													botIsSet = *(--botIsSetIt);
												}
												else {
													botIsSet = *(++botIsSetIt);
												}
											}

											if (sf::Keyboard::getDescription(code) == "Right") {

												if (botIsSetIt != --botSettings.end()) {
													botIsSet = *(++botIsSetIt);
												}
												else {
													botIsSet = *(--botIsSetIt);
												}
											}
										}

										break;
									case 2:

										if (event.key.code != sf::Keyboard::Enter) {
											break;
										}

										if (event.type == sf::Event::KeyReleased) {

											std::string rounds = keyRoundsCount.getString();

											settings[4].second = rounds;
											settings[5].second = botIsSet;

											setSettings(settings);

											Snake snake;
											singleplayer game(snake);

											gameSingleplayerStart(window, game);
										}


										break;
									case 3:

										if (event.key.code == sf::Keyboard::Enter) {
											singleplayerIsStaart = false;
										}
										break;
									}


								}
							}
							keyRoundsCount.setFont(font);
							setControl(keyRoundsCount, roundsCount, 360);
							keyBot.setFont(font);
							setControl(keyBot, botIsSet, 460);

							window.clear();
							window.draw(background);
							window.draw(titul);
							singleplayerMenu.draw();
							window.draw(keyRoundsCount);
							window.draw(keyBot);
							window.display();
						}
						break;
					}
					case 1: {

						bool multyplayer = true;

						// Обрабатывает нажатие кнопки.
						sf::String roundsCount = settings[4].second;

						while (multyplayer) {

							sf::Keyboard::Scancode code = sf::Keyboard::Scancode::Unknown;



							while (window.pollEvent(event))
							{
								// Закрывает окно.
								if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
									window.close();

								// Обрабатывает нажатие кнопки.
								if (event.type == sf::Event::KeyReleased) {

									// Выбор нижестоящей кнопки.
									if (event.key.code == sf::Keyboard::Up) {
										multyplayerMenu.moveUp();
									}

									// Выбор нижестоящей кнопки.
									if (event.key.code == sf::Keyboard::Down) {
										multyplayerMenu.moveDown();
									}

									switch (multyplayerMenu.getSelected()) {

									case 0:
										if (event.type == sf::Event::KeyReleased) {
											code = event.key.scancode;
											if (sf::Keyboard::getDescription(code) >= '1' && sf::Keyboard::getDescription(code) <= '9') {
												roundsCount = sf::Keyboard::getDescription(code);
											}
										}

										break;
									case 1:

										if (event.key.code != sf::Keyboard::Enter) {
											break;
										}

										if (event.type == sf::Event::KeyReleased) {

											std::string rounds = keyRoundsCount.getString();
											std::string bots = keyBot.getString();

											settings[4].second = rounds;

											setSettings(settings);

											multyGame.startTwoPlayaGame(window);
										}


										break;
									case 2:

										if (event.key.code == sf::Keyboard::Enter) {
											multyplayer = false;
										}
										break;
									}


								}
							}
							keyRoundsCount.setFont(font);
							setControl(keyRoundsCount, roundsCount, 450);

							window.clear();
							window.draw(background);
							window.draw(titul);
							multyplayerMenu.draw();
							window.draw(keyRoundsCount);
							window.display();
						}
						break;
					}
					case 2:
						gameMenu_.setPositionY(400);
						gameMenu_.pressButton(name, 0);
						break;
					}
					break;
				}

					  // Меню настроек.
				case 2: {

					switch (gameMenu_.getSelected()) {
					case 0: {
						{

							if (event.key.code != sf::Keyboard::Enter) {
								break;
							}

							bool nickMenu = true;

							// Обрабатывает нажатие кнопки.
							sf::String nick1 = settings[6].second;
							sf::String nick2 = settings[7].second;

							while (nickMenu) {

								sf::Keyboard::Scancode code = sf::Keyboard::Scancode::Unknown;

								while (window.pollEvent(event))
								{
									// Закрывает окно.
									if (event.type == sf::Event::Closed)
										window.close();

									// Обрабатывает нажатие кнопки.
									if (event.type == sf::Event::KeyReleased) {

										// Выбор нижестоящей кнопки.
										if (event.key.code == sf::Keyboard::Up) {
											NicknamesMenu.moveUp();
										}

										// Выбор нижестоящей кнопки.
										if (event.key.code == sf::Keyboard::Down) {
											NicknamesMenu.moveDown();
										}

										switch (NicknamesMenu.getSelected()) {


										case 0: {
											if (event.type != sf::Event::KeyReleased) {
												break;
											}

											auto code = event.key.scancode;

											if (sf::Keyboard::getDescription(code) == "Enter") {
												nick1 = "";
												break;
											}

											if (sf::Keyboard::getDescription(code) == "Backspace") {
												nick1 = nick1.substring(0, nick1.getSize() - 1);
												break;
											}

											if (code >= 0 && code <= 35) {
												nick1 += sf::Keyboard::getDescription(code);
											}
											break;
										}
										case 1: {
											if (event.type != sf::Event::KeyReleased) {
												break;
											}

											auto code = event.key.scancode;

											if (sf::Keyboard::getDescription(code) == "Enter") {
												nick2 = "";
												break;
											}

											if (sf::Keyboard::getDescription(code) == "Backspace") {
												nick2 = nick2.substring(0, nick2.getSize() - 1);
												break;
											}

											if (code >= 0 && code <= 35) {
												nick2 += sf::Keyboard::getDescription(code);
											}

											break;
										}

										case 2:
											if (event.key.code == sf::Keyboard::Enter) {

												settings[6].second = nick1;
												settings[7].second = nick2;

												setSettings(settings);
											}
											break;
										case 3:
											if (event.key.code == sf::Keyboard::Enter) {
												nickMenu = false;
											}
											break;
										}
									}

								}

								nickname1.setFont(font);
								setControl(nickname1, nick1, 360);
								nickname2.setFont(font);
								setControl(nickname2, nick2, 460);

								window.clear();
								window.draw(background);
								window.draw(titul);
								NicknamesMenu.draw();
								window.draw(nickname1);
								window.draw(nickname2);
								window.display();
							}
						}
						break;
					}

					case 1: {

						if (event.key.code != sf::Keyboard::Enter) {
							break;
						}

						bool control = true;

						// Обрабатывает нажатие кнопки.
						sf::String keyU = settings[0].second;
						sf::String keyL = settings[1].second;
						sf::String keyD = settings[2].second;
						sf::String keyR = settings[3].second;
						while (control) {

							sf::Keyboard::Scancode code = sf::Keyboard::Scancode::Unknown;

							while (window.pollEvent(event))
							{
								// Закрывает окно.
								if (event.type == sf::Event::Closed)
									window.close();

								// Обрабатывает нажатие кнопки.
								if (event.type == sf::Event::KeyReleased) {

									// Выбор нижестоящей кнопки.
									if (event.key.code == sf::Keyboard::Up) {
										controlMenu.moveUp();
									}

									// Выбор нижестоящей кнопки.
									if (event.key.code == sf::Keyboard::Down) {
										controlMenu.moveDown();
									}

									switch (controlMenu.getSelected()) {


									case 0:
										if (event.type == sf::Event::KeyReleased) {
											code = event.key.scancode;
											if (sf::Keyboard::getDescription(code) != "Up" && sf::Keyboard::getDescription(code) != "Down") {
												keyU = sf::Keyboard::getDescription(code);
											}
										}

										break;
									case 1:
										if (event.type == sf::Event::KeyReleased) {
											code = event.key.scancode;
											if (sf::Keyboard::getDescription(code) != "Up" && sf::Keyboard::getDescription(code) != "Down") {
												keyL = sf::Keyboard::getDescription(code);
											}
										}

										break;
									case 2:
										if (event.type == sf::Event::KeyReleased) {
											code = event.key.scancode;
											if (sf::Keyboard::getDescription(code) != "Up" && sf::Keyboard::getDescription(code) != "Down") {
												keyD = sf::Keyboard::getDescription(code);
											}
										}

										break;
									case 3:

										if (event.type == sf::Event::KeyReleased) {
											code = event.key.scancode;
											if (sf::Keyboard::getDescription(code) != "Up" && sf::Keyboard::getDescription(code) != "Down") {
												keyR = sf::Keyboard::getDescription(code);
											}
										}

										break;

									case 4:
										if (event.key.code == sf::Keyboard::Enter) {

											std::string Up = keyUp.getString();
											std::string Left = keyLeft.getString();
											std::string Down = keyDown.getString();
											std::string Right = keyRight.getString();

											//std::vector<std::pair<std::string, std::string>> controlSettings;

											settings[0].second = Up;
											settings[1].second = Left;
											settings[2].second = Down;
											settings[3].second = Right;

											setSettings(settings);
										}
										break;
									case 5:
										if (event.key.code == sf::Keyboard::Enter) {
											control = false;
										}
										break;
									}
								}

							}

							keyLeft.setFont(font);
							setControl(keyLeft, keyL, 460);
							keyUp.setFont(font);
							setControl(keyUp, keyU, 360);
							keyDown.setFont(font);
							setControl(keyDown, keyD, 560);
							keyRight.setFont(font);
							setControl(keyRight, keyR, 660);

							window.clear();
							window.draw(background);
							window.draw(titul);
							controlMenu.draw();
							window.draw(keyUp);
							window.draw(keyLeft);
							window.draw(keyDown);
							window.draw(keyRight);
							window.display();
						}
						break;
					}
					case 2: {

						if (event.key.code != sf::Keyboard::Enter) {
							break;
						}

						bool thems = true;
						auto player1ColorIt = std::find(playerColorSettings.begin(), playerColorSettings.end(), settings[9].second);
						auto player2ColorIt = std::find(playerColorSettings.begin(), playerColorSettings.end(), settings[10].second);
						auto appleColorIt = std::find(appleColorSettings.begin(), appleColorSettings.end(), settings[11].second);
						auto botColorIt = std::find(botColorSettings.begin(), botColorSettings.end(), settings[12].second);

						while (thems) {

							sf::RectangleShape player1;
							sf::Texture player1Texture;
							std::string player1TexturePath = "../designe/snake" + player1Color + ".png";
							if (!player1Texture.loadFromFile(player1TexturePath)) return -1;

							player1.setSize(sf::Vector2f(30, 30));
							player1.setPosition(sf::Vector2f(980, 390));
							player1.setTexture(&player1Texture);

							sf::RectangleShape player2;
							sf::Texture player2Texture;
							std::string player2TexturePath = "../designe/snake" + player2Color + ".png";
							if (!player2Texture.loadFromFile(player2TexturePath)) return -1;

							player2.setSize(sf::Vector2f(30, 30));
							player2.setPosition(sf::Vector2f(980, 480));
							player2.setTexture(&player2Texture);

							sf::RectangleShape apple;
							sf::Texture appleTexture;
							std::string appleTexturePath = "../designe/apple" + appleColor + ".png";
							if (!appleTexture.loadFromFile(appleTexturePath)) return -1;

							apple.setSize(sf::Vector2f(30, 30));
							apple.setPosition(sf::Vector2f(980, 570));
							apple.setTexture(&appleTexture);

							sf::RectangleShape bot;
							sf::Texture botTexture;
							std::string botTexturePath = "../designe/bot" + botColor + ".png";
							if (!botTexture.loadFromFile(botTexturePath)) return -1;

							bot.setSize(sf::Vector2f(30, 30));
							bot.setPosition(sf::Vector2f(980, 670));
							bot.setTexture(&botTexture);

							while (window.pollEvent(event))
							{
								// Закрывает окно.
								if (event.type == sf::Event::Closed)
									window.close();

								// Обрабатывает нажатие кнопки.
								if (event.type == sf::Event::KeyReleased) {

									// Выбор нижестоящей кнопки.
									if (event.key.code == sf::Keyboard::Up) {
										themsMenu.moveUp();
									}

									// Выбор нижестоящей кнопки.
									if (event.key.code == sf::Keyboard::Down) {
										themsMenu.moveDown();
									}

									switch (themsMenu.getSelected()) {


									case 0: {

										if (event.type == sf::Event::KeyReleased) {
											if (event.type == sf::Event::KeyReleased) {
												auto code = event.key.scancode;
												if (sf::Keyboard::getDescription(code) == "Left") {

													if (player1ColorIt != playerColorSettings.begin()) {
														player1Color = *(--player1ColorIt);
													}
													else {
														player1ColorIt = --playerColorSettings.end();
														player1Color = *(player1ColorIt);
													}
												}

												if (sf::Keyboard::getDescription(code) == "Right") {

													if (player1ColorIt != --playerColorSettings.end()) {
														player1Color = *(++player1ColorIt);
													}
													else {
														player1ColorIt = playerColorSettings.begin();
														player1Color = *(player1ColorIt);
													}
												}
											}
										}
									}
										  break;
									case 1: {
										if (event.type == sf::Event::KeyReleased) {
											if (event.type == sf::Event::KeyReleased) {
												auto code = event.key.scancode;
												if (sf::Keyboard::getDescription(code) == "Left") {

													if (player2ColorIt != playerColorSettings.begin()) {
														player2Color = *(--player2ColorIt);
													}
													else {
														player2ColorIt = --playerColorSettings.end();
														player2Color = *(player2ColorIt);
													}
												}

												if (sf::Keyboard::getDescription(code) == "Right") {

													if (player2ColorIt != --playerColorSettings.end()) {
														player2Color = *(++player2ColorIt);
													}
													else {
														player2ColorIt = playerColorSettings.begin();
														player2Color = *(player2ColorIt);
													}
												}
											}
										}
									}

										  break;
									case 2: {

										if (event.type == sf::Event::KeyReleased) {
											if (event.type == sf::Event::KeyReleased) {
												auto code = event.key.scancode;
												if (sf::Keyboard::getDescription(code) == "Left") {

													if (appleColorIt != appleColorSettings.begin()) {
														appleColor = *(--appleColorIt);
													}
													else {
														appleColorIt = --appleColorSettings.end();
														appleColor = *(appleColorIt);
													}
												}

												if (sf::Keyboard::getDescription(code) == "Right") {

													if (appleColorIt != --appleColorSettings.end()) {
														appleColor = *(++appleColorIt);
													}
													else {
														appleColorIt = appleColorSettings.begin();
														appleColor = *(appleColorIt);
													}
												}
											}
										}
									}

										  break;

									case 3: {

										if (event.type == sf::Event::KeyReleased) {
											if (event.type == sf::Event::KeyReleased) {
												auto code = event.key.scancode;
												if (sf::Keyboard::getDescription(code) == "Left") {

													if (botColorIt != botColorSettings.begin()) {
														botColor = *(--botColorIt);
													}
													else {
														botColorIt = --botColorSettings.end();
														botColor = *(botColorIt);
													}
												}

												if (sf::Keyboard::getDescription(code) == "Right") {

													if (botColorIt != --botColorSettings.end()) {
														botColor = *(++botColorIt);
													}
													else {
														botColorIt = botColorSettings.begin();
														botColor = *(botColorIt);
													}
												}
											}
										}
									}

										  break;
									case 4: {

										if (event.key.code != sf::Keyboard::Enter)
											break;

										settings[9].second = *(player1ColorIt);
										settings[10].second = *(player2ColorIt);
										settings[11].second = *(appleColorIt);
										settings[12].second = *(botColorIt);

										setSettings(settings);
									}

										  break;

									case 5:
										if (event.key.code == sf::Keyboard::Enter) {
											thems = false;
										}
										break;
									}
								}
							}

							window.clear();
							window.draw(background);
							window.draw(titul);
							themsMenu.draw();
							window.draw(player1);
							window.draw(player2);
							window.draw(apple);
							window.draw(bot);
							window.display();
						}
					}
						  break;
					case 3: {

						if (event.key.code != sf::Keyboard::Enter) {
							break;
						}

						bool field = true;


						while (field)
						{
							while (window.pollEvent(event))
							{
								// Закрывает окно.
								if (event.type == sf::Event::Closed)
									window.close();

								// Обрабатывает нажатие кнопки.
								if (event.type == sf::Event::KeyReleased) {

									// Выбор нижестоящей кнопки.
									if (event.key.code == sf::Keyboard::Up) {
										fieldMenu.moveUp();
									}

									// Выбор нижестоящей кнопки.
									if (event.key.code == sf::Keyboard::Down) {
										fieldMenu.moveDown();
									}

									if (event.key.code != sf::Keyboard::Enter)
										break;

									switch (fieldMenu.getSelected()) {
									case 0:
										settings[8].second = "1";
										break;
									case 1:
										settings[8].second = "2";
										break;
									case 2:
										settings[8].second = "3";
										break;
									case 3:
										setSettings(settings);
										field = false;
										break;
									}
								}
							}


							window.clear();
							window.draw(background);
							window.draw(titul);
							fieldMenu.draw();
							window.display();
						}

						break;
					}
					case 4: {

						if (event.key.code != sf::Keyboard::Enter) {
							break;
						}

						gameMenu_.setPositionY(400);
						gameMenu_.pressButton(name, 0);
						break;
					}
					}
					break;
				}

					  // Меню "about".
				case 3: {

					switch (gameMenu_.getSelected()) {
					case 0:

						if (event.key.code == sf::Keyboard::Enter) {
							about = false;
							gameMenu_.setPositionY(400);
							gameMenu_.pressButton(name, 0);
						}

						break;
					}
				}
					  break;


				case 4:
					if (event.key.code != sf::Keyboard::Enter) {
						break;
					}

					switch (gameMenu_.getSelected()) {
					case 0:
						gameMenu_.setPositionY(400);
						gameMenu_.pressButton(name, 0);
						break;
					case 1:
						window.close();
						return 0;
					}
				}

			}
		}

		// Отрисовка.
		window.clear();
		window.draw(background);

		if (gameMenu_.getMode() == 4) {
			window.draw(exit1);
			window.draw(exit2);
		}

		if (about) {
			window.draw(developers);
			window.draw(owner);
			window.draw(developer);
			window.draw(kanye);
		}

		window.draw(titul);
		gameMenu_.draw();
		window.display();
	}

	return 0;
}

void InitText(sf::Text& mtext, float xpos, float ypos, sf::String str, int size_font,
	sf::Color menu_text_color, int bord, sf::Color border_color)
{
	mtext.setCharacterSize(size_font);
	mtext.setPosition(xpos, ypos);
	mtext.setString(str);
	mtext.setFillColor(menu_text_color);
	mtext.setOutlineThickness(bord);
	mtext.setOutlineColor(border_color);
}

void setControl(sf::Text& key, sf::String keyName, float ypos)
{
	InitText(key, 1000, ypos, keyName, 55, sf::Color::Yellow, 3, sf::Color(40, 40, 40));
}

int gameSingleplayerStart(sf::RenderWindow& window, singleplayer game)
{
	int result = game.startSingleplayer(window);
	switch (result) {
	case 2 :
			gameSingleplayerStart(window, game);
	case 0 :
		return 1;
	}
	return 1;
}