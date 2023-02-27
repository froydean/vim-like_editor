#include "model.h"
#include "view.h"

#define LINE_NUM_SPACE_DEFAULT 4
#define LINE_NUM_SPACE_MAX 100

View::View(Model* model_pt) {

	model_ = model_pt;
	first_line_displayed_ = 0;
	first_shown_sym_index_ = 0;
	first_line_logical_ = 0;
	line_numbering_ = false;
	line_num_space_ = LINE_NUM_SPACE_DEFAULT;
	move_cursor(0, 0);
}

void View::calc_first_shown_line_n_index() {
	int index = 0;
	first_line_logical_ = 0; //counter of logical lines
	unsigned int i = 0; //counter actual lines
	while (i < first_line_displayed_) {
		if (model_->chars_in_line_count[first_line_logical_] + line_num_space_ > max_x) {
			int tmp = model_->chars_in_line_count[first_line_logical_] / (max_x - line_num_space_); //count of lines in line - 1
			if (i + tmp >= first_line_displayed_) { //do not checked if equal
				index += (first_line_displayed_ - i) * (max_x - line_num_space_);
				break;
			}
			else {
				i += tmp;
			}
		}
		index += model_->chars_in_line_count[first_line_logical_] + 1;
		first_line_logical_++;
		i++;
	}
	first_shown_sym_index_ = index;
}

void View::calculate_actual_win_yx(unsigned int& cur_window_actual_line, unsigned int& cur_window_actual_pos) {

	unsigned int sub_lines_num = 0;
	//line overflowing resolve: calc actual window's line and pos
	for (unsigned int i = 0; i < model_->cur_line_; i++) {
		if (model_->chars_in_line_count[i] + line_num_space_ > max_x) {
			sub_lines_num = model_->chars_in_line_count[i] / (max_x - line_num_space_);
			cur_window_actual_line += sub_lines_num;
		}
		cur_window_actual_line++;
	}
	//cur_line now is the number of physic window's X - start of the cur_line
	//add nuber of physic lines till the pos_
	cur_window_actual_line += model_->cur_pos_ / (max_x - line_num_space_);
	//calc phys podition
	cur_window_actual_pos = model_->cur_pos_ % (max_x - line_num_space_) + line_num_space_;
}

void View::print_status_bar() {
	//[Mode][line/position][File]
	if (model_->mode_ == "Navigation mode") {
		print(max_y - 1, 0, "--NAVIGATION--"); //14 sym
	}
	else if (model_->mode_ == "Insert mode") {
		print(max_y - 1, 0, "--INSERT--");
	}
	else if (model_->mode_ == "Search mode") { //status bar dissappears to enter the search text
		//print(max_y - 1, 0, "--SEARCH--");
		return;
	}
	else if (model_->mode_ == "Command mode") {
		//print(max_y - 1, 0, "--COMMAND--");
		return;
	}
	print(max_y - 1, 15, "[File: %20s]", model_->file_.c_str()); //28 sym
	print(max_y - 1, 44, "[Pos: %6d/%6d]", model_->cur_line_, model_->cur_pos_, 0, 0); //20 sym

	move_cursor(0, 0);
}

//not working with over 3sym num
void View::print_text_on_screen() {
	unsigned int line = first_line_logical_;
	char line_str[LINE_NUM_SPACE_MAX] = "";
	
	unsigned int index = first_shown_sym_index_;
	unsigned int i = index;
	
	char line_num_space_str[LINE_NUM_SPACE_MAX] = "";
	string format = "%";
	itoa(line_num_space_-1, line_num_space_str, 10);
	format.append(line_num_space_str);
	format.append("s ");
	
	if (line_numbering_) {
		itoa(line, line_str, 10);
		print(0, 0, format.c_str(), line_str); //LINE_NUM_SPACE-1
	}
	get_yx();
	while (model_->buffer_[index] && cur_y < max_y - 1) {
		replace_with_readable_sym(index);
		if (model_->buffer_[index] == '\n') {
			print_symbol('\n');
			index++;
			line++;
			if (line_numbering_) {
				itoa(line, line_str, 10);
				get_yx();
				print(cur_y, 0, format.c_str(), line_str);
			}
		}
		else {
			if (line_numbering_) {
				get_yx();
				if (cur_x == 0) {
					for (int i = 0; i < line_num_space_; i++) {
						print_symbol(' ');
					}
				}
			}
			print_symbol(model_->buffer_[index]);
			index++;
		}
		get_yx();
	}
}

void View::scroll(int cur_window_actual_line) {
	if (cur_window_actual_line < first_line_displayed_) {
		scroll_up(first_line_displayed_- cur_window_actual_line);
	}
	if (cur_window_actual_line - first_line_displayed_ > max_y - 2) {
		scroll_down((cur_window_actual_line - first_line_displayed_) - max_y + 2);
	}
}

void View::scroll_down(unsigned int lines_skip) {
	for (unsigned int i = 0; i < lines_skip; i++)
		first_line_displayed_++;
}

void View::scroll_up(unsigned int lines_skip) {
	for (unsigned int i = 0; i < lines_skip; i++)
		first_line_displayed_--;
}

void View::replace_with_readable_sym(int index) {
	if (model_->buffer_[index] != '\n' && (model_->buffer_[index] < ' ' || model_->buffer_[index] > '~')) {
		model_->buffer_.erase(index, 1);
		model_->buffer_.insert(index, 1, ' ');
	}
}

void View::print_screen() {

	if (line_numbering_) {
		unsigned int len_line_num = trunc(log10(model_->last_line_num_)) + 1;
		line_num_space_ = len_line_num + 1 > LINE_NUM_SPACE_DEFAULT?len_line_num+1: LINE_NUM_SPACE_DEFAULT; // space = length of the last_num
		//line_num_space_ = 4; //hardcode
	}
	else {
		line_num_space_ = 0;
	}
	unsigned int cur_window_actual_line = 0;
	unsigned int cur_window_actual_pos = model_->cur_pos_;
	calculate_actual_win_yx(cur_window_actual_line, cur_window_actual_pos);
	clear_screen();
	get_max_win_param();
	scroll(cur_window_actual_line);
	calc_first_shown_line_n_index();
	print_text_on_screen();
	print_status_bar();
	move_cursor(cur_window_actual_line - first_line_displayed_, cur_window_actual_pos);
	refresh_screen();
}

//do not handle search_word more than max_x
void View::print_search_screen(chtype_adapted search_mode) {
	clear_last_line();
	print(max_y - 1, 0, search_mode=='/'?"/":"?");
	if (!model_->search_word_.empty()) {
		print(max_y - 1, 1, model_->search_word_.c_str());
	}
	move_cursor(max_y - 1, model_->search_word_.size()+1);
	refresh_screen();
}
void View::print_command_screen() {
	clear_last_line();
	print(max_y - 1, 0, ":");
	if (!model_->cmd_.empty()) {
		print(max_y - 1, 1, model_->cmd_.c_str());
	}
	move_cursor(max_y - 1, model_->cmd_.size() + 1);
	refresh_screen();
}

void View::print_not_found() {
	clear_last_line();
	print(max_y - 1, 0, "Not found");
	move_cursor(0,0);
	refresh_screen();
}

void View::print_help() {
	clear_screen();
	print(0, 0, "[Navigation mode]");
	print(1, 0, "    ^    - Move cursor to start of line.");
	print(2, 0, "    $    - Move cursor to end of line.");
	print(3, 0, "    w    - Move the cursor to the end of the word to the right of the cursor.");
	print(4, 0, "    b    - Move the cursor to the beginning of a word to the left of the cursor.");
	print(5, 0, "    gg   - Move the cursor to the first page of the text.");
	print(6, 0, "    G    - Move the cursor to the last page of the text.");
	print(7, 0, "    NG   - Move to line with number N.");
	print(8, 0, "    x    - Delete character after cursor.");
	print(9, 0, "    diw  - Delete the word under the cursor, including the space to the right.");
	print(10, 0, "    dd   - Cut the current line.");
	print(11, 0, "    yy   - Copy current line.");
	print(12, 0, "    yw   - Copy word under cursor.");
	print(13, 0, "    p    - Paster after cursor.");
	print(15, 0, "Press any button to continue...");
}
