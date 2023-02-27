#ifndef ADAPTER_H
#define ADAPTER_H
#include "curses.h"
#include "panel.h"
#include <iostream>
#include <vector>

typedef chtype chtype_adapted;

class Adapter {

public:
	int max_y, max_x;
	int cur_y, cur_x;

	Adapter();
	~Adapter();

	void move_cursor(int y, int x) const;
	void print_symbol(char symbol) const;

	void get_max_win_param();

	void print(int y, int x, const char* str) const;
	void print(int y, int x, const char* str, std::string cmd) const;
	void print(int y, int x, const char* str, unsigned int cur_line, unsigned int max_lines, unsigned int cur_pos, unsigned int max_line_pos) const;

	void clear_screen() const;

	void refresh_screen() const;
	void clear_last_line() const;
	void get_yx();
};
#endif //ADAPTER_H