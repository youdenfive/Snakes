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
		float _menu_x;		// X-���� ���������� ����.
		float _menu_y;		// Y-���� ���������� ����.
		int _menu_step;		// ���������� ����� �������� ����.
		int _max_menu;		// ������������ ���������� ��������� ����.
		int _size_font;		// ������ ������.
		int _menu_selected;	// ��������� ������� ����.
		int _menuMode = 0;  // ����� � ������� ��������� ����.

		sf::Font _font;		// ����� ����.
		sf::Text* _menu;		// ������ ��������� ����.

		sf::Color _unselected_color = sf::Color(40, 40, 40);	// ���� �� ���������� �������� ����.
		sf::Color _selected_color = sf::Color::Yellow;		// ���� ���������� �������� ����.
		sf::Color _border_color = sf::Color::Yellow;			// ���� ��������� ������.

		// ��������� �������� ������ ����.
		void setInitText(sf::Text& mtext, sf::String str, float xpos, float ypos);

		sf::RenderWindow& _window; // ������ �� ����������� ����.
};