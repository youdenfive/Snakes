#pragma once
#include <SFML/Graphics.hpp>

class gameMenu {

	public:
		gameMenu(sf::RenderWindow& window, float menuX, float menuY, std::vector<sf::String> name, int menu_step = 80, int font_size = 60);

		~gameMenu()
		{
			delete[] _menu;
		};

		void draw();

		void moveUp();

		void moveDown();

		void setColor(sf::Color unselected_color, sf::Color selected_color, sf::Color border_color);

		void alignTextMenu(int posX);

		int getSelected()
		{
			return _menu_selected;
		};

		void setMenu(std::vector<sf::String> name);

		void setMode(int mode)
		{
			_menuMode = mode;
		};

		int getMode()
		{
			return _menuMode;
		}

		void pressButton(std::vector<sf::String> name, int mode, int align = 1);

	private:
		float _menu_x;		// X-овая координата меню.
		float _menu_y;		// Y-овая координата меню.
		int _menu_step;		// Расстояние между пунктами меню.
		int _max_menu;		// Максимальное количество элементов меню.
		int _size_font;		// Размер шрифта.
		int _menu_selected;	// Выбранный элемент меню.
		int _menuMode = 0;  // Режим в котором находится меню.

		sf::Font _font;		// Шрифт меню.
		sf::Text* _menu;		// Массив элементов меню.

		sf::Color _unselected_color = sf::Color(40, 40, 40);	// Цвет не выбранного элемента меню.
		sf::Color _selected_color = sf::Color::Yellow;		// Цвет выбранного элемента меню.
		sf::Color _border_color = sf::Color::Yellow;			// Цвет окантовки шрифта.

		// Установка настроек текста меню.
		void setInitText(sf::Text& mtext, sf::String str, float xpos, float ypos);

		sf::RenderWindow& _window; // Ссылка на графическое окно.
};