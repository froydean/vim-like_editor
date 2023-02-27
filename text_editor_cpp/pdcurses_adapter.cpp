#include "pdcurses_adapter.h"

Adapter::Adapter() {
	initscr(); //pdcurses.lib init
	noecho(); //do not print user's input
	keypad(stdscr, TRUE); //hotkeys

	if (has_colors() == FALSE) { //color handle
		endwin();
		std::cout << "Your terminal does not support color" << std::endl;
	}
	use_default_colors(); //stdscr background no color
	start_color();
	getmaxyx(stdscr, max_y, max_x);
	getyx(stdscr, cur_y, cur_x);
}

Adapter::~Adapter() {
	endwin();
}

void Adapter::move_cursor(int y, int x) const {
	move(y, x);
}

void Adapter::print_symbol(char symbol) const {
	addch(symbol);
}

void Adapter::get_max_win_param() {
	getmaxyx(stdscr, max_y, max_x);
}

void Adapter::print(int y, int x, const char* str) const {
	mvprintw(y, x, str);
}

void Adapter::print(int y, int x, const char* str, std::string cmd) const {
	mvprintw(y, x, str, cmd.c_str());
}

void Adapter::print(int y, int x, const char* str, unsigned int cur_line, unsigned int max_lines, unsigned int cur_pos, unsigned int max_line_pos) const {
	mvprintw(y, x, str, cur_line, max_lines, cur_pos, max_line_pos);
}

void Adapter::clear_screen() const {
	clear();
}

void Adapter::refresh_screen() const {
	refresh();
}

void Adapter::clear_last_line() const {
	move(max_y-1, 0);
	clrtoeol();
}

void Adapter::get_yx() {
	getyx(stdscr, cur_y, cur_x);
}