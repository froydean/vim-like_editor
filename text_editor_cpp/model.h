#ifndef MODEL_H
#define MODEL_H
#include "my_string_class_header.h"
#include "pdcurses_adapter.h"
#include <vector>
#include <fstream>

class View;

class Model {

private:
	char* copy_file_to_buffer(const char* name, int& len);
	void update_lines();
	unsigned int get_index_of_cur_pos();

public:

	View* view_;
	MyString file_;
	MyString cmd_;
	MyString search_word_;
	MyString mode_;
	unsigned int cur_line_; //count from 0
	unsigned int cur_pos_; //count from 0
	vector<int> chars_in_line_count; //count from 1, index = line number
	unsigned int last_line_num_; //count from 0

	MyString buffer_;
	MyString buffer_for_copying_;

	Model();
	~Model();

	void cur_to_left();
	void cur_to_right();
	void cur_to_up();
	void cur_to_down();
	void page_up();
	void page_down();
	void backspace_key();
	void del_key();
	void insert_sym(chtype_adapted sym);
	void del_word();
	void del_line();
	void copy_line();
	void copy_word();
	void go_to_line(unsigned int num);
	void to_begin_of_line();
	void to_end_of_line();
	void replace_sym(chtype_adapted symbol);
	void cur_ctrl_home();
	void cur_ctrl_end();
	void cur_to_end_of_word();
	void cur_to_begin_of_word();
	void paste();
	void open_file();
	void save_to_file();
	void save_to_new_file();
	void search_forward();
	void search_backward();
	void help();

	void set_line_numbering();
	void unset_line_numbering();

};

#endif //MODEL_H