#ifndef VIEW_H
#define VIEW_H
#include <iostream>
#include<cmath>
#include "my_string_class_header.h"
#include "pdcurses_adapter.h"


class Model;

class View : public Adapter {

private:
	Model* model_;
	unsigned int first_line_displayed_; //actual
	unsigned int first_shown_sym_index_;
	unsigned int first_line_logical_;
	unsigned int line_num_space_;
	
	void calc_first_shown_line_n_index();
	void calculate_actual_win_yx(unsigned int & cur_window_actual_line, unsigned int & cur_window_actual_pos);
	void print_status_bar();
	void print_text_on_screen();
	void scroll(int cur_window_actual_line);
	void scroll_down(unsigned int lines_skip);
	void scroll_up(unsigned int lines_skip);
	void replace_with_readable_sym(int index);

public:
	
	bool line_numbering_;

	View(Model * model_);
	
	void print_screen();
	void print_search_screen(chtype_adapted search_mode);
	void print_command_screen();
	void print_not_found();
	void print_help();
};
#endif //VIEW_H