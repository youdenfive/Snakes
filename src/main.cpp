#include <SFML/Graphics.hpp>
#include "gameMenu.h"
#include "singlepalyer.h"
#include <iostream>
#include <string>
#include "settings.h"

/**
*   �������������� �������� ���� � ����.
* 
*   @param sf::Text& mtext           �������������� �����.
*   @param float xpos                X-���� ������� ������.
*   @param float ypos                Y-���� ������� ������.
*   @param sf::String str            ���������� ������.
*   @param int size_font             ������ ������.
*   @param sf::Color menu_text_color ���� ������.
*   @param int bord                  ������� �������.
*   @param sf::Color border_color    ���� �������.
*
*   @return void
*/
void InitText(sf::Text& mtext, float xpos, float ypos, sf::String str, int size_font = 60,
	sf::Color menu_text_color = sf::Color::Black, int bord = 0, sf::Color border_color = sf::Color::White);

void setControl(sf::Text& key, sf::String keyName, float ypos);

int gameSingleplayerStart(sf::RenderWindow& window, singleplayer game);

int main()
{
	// ������� ���� ����.
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SNAKES", sf::Style::Fullscreen);

	window.setFramerateLimit(60);

	// ������ ������ ���������.
	window.setMouseCursorVisible(false);

	// ������� ����
	float width = sf::VideoMode::getDesktopMode().width;
	float height = sf::VideoMode::getDesktopMode().height;

	// ������������� ������ ��� ����.
	sf::RectangleShape background(sf::Vector2f(width, height));
	sf::Texture texture_window;
	if (!texture_window.loadFromFile("../designe/background_menu.jpg")) return 4;
	background.setTexture(&texture_window);

	// ������������� �����.
	sf::Font font;
	if (!font.loadFromFile("../designe/font/menuFont.ttf")) return 5;

	// ������������� ��������� � ������� ����.
	sf::Text titul;
	titul.setFont(font);
	InitText(titul, 580, 50, "SNAKES", 150, sf::Color(40,40,40), 3, sf::Color::Yellow);

	// �������������� ������� ����.
	std::vector<sf::String> name = {"START", "SETTINGS", "ABOUT", "EXIT" };
	gameMenu gameMenu_(window, 950, 350, name, 130, 100);
	gameMenu_.alignTextMenu(1);
	
	// �������������� ����� ��������� ����
	std::vector<sf::String> startGameName = { "SINGLEPLAYER", "MULTIPLAYER", "GO BACK" };
	std::vector<sf::String> settingsGameName = { "NICKNAME", "CONTROL", "THEME", "FIELD SIZE", "GO BACK"};
	std::vector<sf::String> aboutGameName = { "GO BACK" };

	// �������������� ���� ����������.
	std::vector<sf::String> controlName = { "UP", "LEFT", "DOWN", "RIGHT", "APPLY", "GO BACK" };
	gameMenu controlMenu(window, 950, 350, controlName, 100, 70);
	controlMenu.alignTextMenu(2);

	// �������������� ���� ��������� ����.
	std::vector<sf::String> singleplayerName = { "ROUNDS", "BOTS", "PLAY", "GO BACK" };
	gameMenu singleplayerMenu(window, 950, 350, singleplayerName, 100, 70);
	singleplayerMenu.alignTextMenu(2);

	// �������������� ���� ��������� ����� ������.
	std::vector<sf::String> NicknamesName = { "PLAYER1", "PLAYER2", "SAVE", "GO BACK"};
	gameMenu NicknamesMenu(window, 950, 350, NicknamesName, 100, 70);
	NicknamesMenu.alignTextMenu(2);

	// �������������� ���� ��������� ����� ������.
	std::vector<sf::String> fieldName = { "BIG", "MEDIUM", "SMALL", "GO BACK" };
	gameMenu fieldMenu(window, 950, 350, fieldName, 100, 70);
	fieldMenu.alignTextMenu(1);


	sf::Text nickname1;
	sf::Text nickname2;
	sf::Text keyUp;
	sf::Text keyLeft;
	sf::Text keyDown;
	sf::Text keyRight;

	sf::Text keyRoundsCount;
	sf::Text keyBotsCount;

	sf::String keyName;
	Snake snake;
	singleplayer game(snake);

	bool isSettings = false;

	// ������������ ����.
	while (window.isOpen())
	{
		sf::Event event;

		auto settings = getSettings();

		// ������������ �������.
		while (window.pollEvent(event))
		{
			// ��������� ����.
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
				window.close();

			// ������������ ������� ������.
			if (event.type == sf::Event::KeyReleased) {

				// ����� ����������� ������.
				if (event.key.code == sf::Keyboard::Up) {
					gameMenu_.moveUp();
				}

				// ����� ����������� ������.
				if (event.key.code == sf::Keyboard::Down) {
					gameMenu_.moveDown();
				}


				switch (gameMenu_.getMode()) {

					// ������� ����.
				case 0:

					// ������������ ������� ������.
					if (event.key.code != sf::Keyboard::Enter) {
						break;
					}

						switch (gameMenu_.getSelected()) {
						case 0:
							gameMenu_.pressButton(startGameName, 1);
							break;
						case 1:
							gameMenu_.pressButton(settingsGameName, 2);
							break;
						case 2:
							gameMenu_.pressButton(aboutGameName, 3);
							break;
						case 3:
							window.close();
							break;
						}
					
					break;

					// ���� ������ ����.
				case 1: {

					// ������������ ������� ������.
					if (event.key.code != sf::Keyboard::Enter) {
						break;
					}

					switch (gameMenu_.getSelected()) {
					case 0: {

						bool singleplayer = true;

						// ������������ ������� ������.
						sf::String roundsCount = settings[4].second;
						sf::String botsCount = settings[5].second;

						while (singleplayer) {

							sf::Keyboard::Scancode code = sf::Keyboard::Scancode::Unknown;



							while (window.pollEvent(event))
							{
								// ��������� ����.
								if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
									window.close();

								// ������������ ������� ������.
								if (event.type == sf::Event::KeyReleased) {

									// ����� ����������� ������.
									if (event.key.code == sf::Keyboard::Up) {
										singleplayerMenu.moveUp();
									}

									// ����� ����������� ������.
									if (event.key.code == sf::Keyboard::Down) {
										singleplayerMenu.moveDown();
									}

									switch (singleplayerMenu.getSelected()) {

									case 0:
										if (event.type == sf::Event::KeyReleased) {
											code = event.key.scancode;
											if (sf::Keyboard::getDescription(code) >= '0' && sf::Keyboard::getDescription(code) <= '9') {
												roundsCount = sf::Keyboard::getDescription(code);
											}
										}

										break;
									case 1:
										if (event.type == sf::Event::KeyReleased) {
											code = event.key.scancode;
											if (sf::Keyboard::getDescription(code) >= '0' && sf::Keyboard::getDescription(code) <= '9') {
												botsCount = sf::Keyboard::getDescription(code);
											}
										}

										break;
									case 2:

										if (event.key.code != sf::Keyboard::Enter) {
											break;
										}

											if (event.type == sf::Event::KeyReleased) {

												std::string rounds = keyRoundsCount.getString();
												std::string bots = keyBotsCount.getString();

												//std::vector<std::pair<std::string, std::string>> controlSettings;

												settings[4].second = rounds;
												settings[5].second = bots;

												setSettings(settings);

												gameSingleplayerStart(window, game);
											}
										

										break;
									case 3:

										if (event.key.code == sf::Keyboard::Enter) {
											singleplayer = false;
										}
										break;
									}


								}
							}
							keyRoundsCount.setFont(font);
							setControl(keyRoundsCount, roundsCount, 360);
							keyBotsCount.setFont(font);
							setControl(keyBotsCount, botsCount, 460);

							window.clear();
							window.draw(background);
							window.draw(titul);
							singleplayerMenu.draw();
							window.draw(keyRoundsCount);
							window.draw(keyBotsCount);
							window.display();
						}
						break;
					}
					case 1:break;
					case 2:
						gameMenu_.pressButton(name, 0);
						break;
					}
						break;
					}

					// ���� ��������.
				case 2: {

					switch (gameMenu_.getSelected()) {
					case 0: {
						{

							if (event.key.code != sf::Keyboard::Enter) {
								break;
							}

							bool nickMenu = true;

							// ������������ ������� ������.
							sf::String nick1 = settings[6].second;
							sf::String nick2 = settings[7].second;

							while (nickMenu) {

								sf::Keyboard::Scancode code = sf::Keyboard::Scancode::Unknown;

								while (window.pollEvent(event))
								{
									// ��������� ����.
									if (event.type == sf::Event::Closed)
										window.close();

									// ������������ ������� ������.
									if (event.type == sf::Event::KeyReleased) {

										// ����� ����������� ������.
										if (event.key.code == sf::Keyboard::Up) {
											NicknamesMenu.moveUp();
										}

										// ����� ����������� ������.
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
												nick2 = nick2.substring(0, nick1.getSize() - 1);
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

						// ������������ ������� ������.
						sf::String keyU = settings[0].second;
						sf::String keyL = settings[1].second;
						sf::String keyD = settings[2].second;
						sf::String keyR = settings[3].second;
						while (control) {

							sf::Keyboard::Scancode code = sf::Keyboard::Scancode::Unknown;

							while (window.pollEvent(event))
							{
								// ��������� ����.
								if (event.type == sf::Event::Closed)
									window.close();

								// ������������ ������� ������.
								if (event.type == sf::Event::KeyReleased) {

									// ����� ����������� ������.
									if (event.key.code == sf::Keyboard::Up) {
										controlMenu.moveUp();
									}

									// ����� ����������� ������.
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
					case 2:break;
					case 3: {

						if (event.key.code != sf::Keyboard::Enter) {
							break;
						}

						bool field = true;


						while (field)
						{
							while (window.pollEvent(event))
							{
								// ��������� ����.
								if (event.type == sf::Event::Closed)
									window.close();

								// ������������ ������� ������.
								if (event.type == sf::Event::KeyReleased) {

									// ����� ����������� ������.
									if (event.key.code == sf::Keyboard::Up) {
										fieldMenu.moveUp();
									}

									// ����� ����������� ������.
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

						gameMenu_.pressButton(name, 0);
						break;
					}
					}
					break;
				}

					// ���� "about".
				case 3:
					switch (gameMenu_.getSelected()) {
					case 0:
						gameMenu_.pressButton(name, 0);
						break;
					}
					break;
				}
				

			
			}
		}

		// ���������.
		window.clear();
		window.draw(background);
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