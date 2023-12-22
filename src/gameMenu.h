#pragma once
#include <SFML/Graphics.hpp>

class gameMenu {

	public:

		/**
		*	�����������.
		* 
		*	@param sf::RenderWindow& window		�������������� ����.
		* 	@param float menuX					X-���� ������� ������.
		* 	@param float menuY					Y-���� ������� ������.
		* 	@param std::vector<sf::String> name ���������� ���� ������ ����.
		* 	@param int menu_step				������ ����� ��������.
		* 	@param int font_size				������ ������.
		*/
		gameMenu(sf::RenderWindow& window, float menuX, float menuY, std::vector<sf::String> name, int menu_step = 80, int font_size = 60);

		/// ����������.
		~gameMenu()
		{
			//delete[] _menu;
		};

		/**
		*	��������� ����.
		* 
		*	@return void
		*/
		void draw();

		/**
		*	����� ����������� ������.
		*
		*	@return void
		*/
		void moveUp();

		/**
		*	����� ����������� ������.
		*
		*	@return void
		*/
		void moveDown();

		/**
		*	��������� ����� ����.
		*
		*	@param sf::Color unselected_color ���� ����������� ������.
		* 	@param sf::Color selected_color	  ���� ��������� ������.
		* 	@param sf::Color border_color	  ���� �������.
		* 
		*	@return void
		*/
		void setColor(sf::Color unselected_color, sf::Color selected_color, sf::Color border_color);

		/**
		*	������������ ����.
		* 
		*	@param integer posX ��� ������������ �� �����������.
		* 
		*	@return void
		*/
		void alignTextMenu(int posX);

		/**
		*	���������� ����� ���������� ������ ����.
		* 
		*	@return integer
		*/
		int getSelected()
		{
			return _menu_selected;
		};

		/**
		*	������������� ������ ����.
		* 
		*	@param std::vector<sf::String> name ���������� ������.
		* 
		*	@return void
		*/
		void setMenu(std::vector<sf::String> name);

		/**
		*	������������� ����� ����.
		* 
		*	@param integer mode ����� ����.
		* 
		*	@return void
		*/
		void setMode(int mode)
		{
			_menuMode = mode;
		};

		/**
		*	���������� ����� ����.
		*
		*	@return integer
		*/
		int getMode()
		{
			return _menuMode;
		}

		/**
		*	������ ���������� ����.
		* 
		*	@param std::vector<sf::String> name ���������� ������.
		*	@param int mode				        ����� ����.
		*	@param int align					��� ������������.
		* 
		*	@return void
		*/
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

		/**
		*	������������� ����� ������.
		* 
		*	@param sf::Text& mtext	�������������� ������.
		* 	@param sf::String str	���������� ������.
		* 	@param float xpos		X-���� ���������� ������.
		*  	@param float ypos		Y-���� ���������� ������.
		* 
		*	@return void
		*/
		void setInitText(sf::Text& mtext, sf::String str, float xpos, float ypos);

		sf::RenderWindow& _window; // ������ �� ����������� ����.
};
