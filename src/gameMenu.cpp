#include "gameMenu.h"
#include <vector>

void gameMenu::setInitText(sf::Text& mtext, sf::String str, float xpos, float ypos)
{
	mtext.setFont(_font);
	mtext.setString(str);
	mtext.setPosition(xpos, ypos);
	mtext.setFillColor(_unselected_color);
	mtext.setCharacterSize(_size_font);
	mtext.setPosition(xpos, ypos);
	mtext.setOutlineThickness(3);
	mtext.setOutlineColor(_border_color);
}

gameMenu::gameMenu(sf::RenderWindow& window, float menuX, float menuY, std::vector<sf::String> name, int menu_step, int font_size):
	_window(window), _menu_x(menuX), _menu_y(menuY), _menu_step(menu_step), _size_font(font_size)
{
	if (!_font.loadFromFile("../designe/font/menuFont.ttf")) exit(15);

	setMenu(name);
}

void gameMenu::setMenu(std::vector<sf::String> name)
{
	delete[] _menu;

	_max_menu = std::size(name);
	_menu_selected = 0;
	_menu = new sf::Text[std::size(name)];

	for (int i = 0; i < _max_menu; ++i) {
		setInitText(_menu[i], name[i], _menu_x, _menu_y + i * _menu_step);
	}

	_menu[_menu_selected].setFillColor(_selected_color);
}
void gameMenu::setColor(sf::Color unselected_color, sf::Color selected_color, sf::Color border_color)
{
	_unselected_color = unselected_color;
	_selected_color   = selected_color;
	_border_color     = border_color;
}

void gameMenu::alignTextMenu(int posX)
{
	int nullx = 0;

	for (int i = 0; i < _max_menu; ++i) {
		switch (posX)
		{
		case 0 :
			nullx = 0;
			break;
		case 1:
			nullx = _menu[i].getLocalBounds().width / 2;
			break;
		case 2:
			nullx = _menu[i].getLocalBounds().width;
			break;
		}

		_menu[i].setPosition(_menu[i].getPosition().x - nullx, _menu[i].getPosition().y);
	}
}

void gameMenu::moveUp()
{
	if (_menu_selected != 0) {
		--_menu_selected;
		_menu[_menu_selected + 1].setFillColor(_unselected_color);
		_menu[_menu_selected].setFillColor(_selected_color);
	}
	else {
		_menu[_menu_selected].setFillColor(_unselected_color);
		_menu_selected = _max_menu - 1;
		_menu[_menu_selected].setFillColor(_selected_color);
	}
}

void gameMenu::moveDown()
{
	if (_menu_selected != _max_menu - 1) {
		++_menu_selected;
		_menu[_menu_selected - 1].setFillColor(_unselected_color);
		_menu[_menu_selected].setFillColor(_selected_color);
	}
	else {
		_menu[_menu_selected].setFillColor(_unselected_color);
		_menu_selected = 0;
		_menu[_menu_selected].setFillColor(_selected_color);
	}
}

void gameMenu::draw()
{
	for (int i = 0; i < _max_menu; ++i) {
		_window.draw(_menu[i]);
	}
}

void gameMenu::pressButton(std::vector<sf::String> name, int mode, int align)
{
	this->setMenu(name);
	this->setMode(mode);
	this->alignTextMenu(align);
}