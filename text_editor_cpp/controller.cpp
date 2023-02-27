#include "controller.h"

Controller::Controller() {
	exit_program_ = false;
	model_ = new Model();
}

Controller::~Controller() {
		delete model_;
}

void Controller::navigation_commands() {
	if (model_->cmd_ == "diw") {
		model_->del_word();
		return;
	}
	if (model_->cmd_ == "dd") {
		model_->copy_line();
		model_->del_line();
		model_->backspace_key();
		return;
	}
	if (model_->cmd_ == "y") {
		model_->copy_line(); //if space, then the word before it, if '\n' or several spaces nothing
		return;
	}
	if (model_->cmd_ == "yw") {
		model_->copy_word(); //if copiyng \n buffer_for_copying simple clears
		return;
	}
	if (model_->cmd_[model_->cmd_.length() - 1] == 'G') {
		model_->cmd_.erase(model_->cmd_.length() - 1, 1);
		int num = atoi(model_->cmd_.c_str());
		model_->go_to_line(num);
		return;
	}
}

void Controller::navigation_mode() {	
	while (!exit_program_) {
		model_->mode_ = "Navigation mode";
		model_->view_->print_screen();
		chtype_adapted symbol = getch();
		switch (symbol) {
		case 'i':
			write_mode();
			break;
		case '^': case KEY_HOME:
			model_->to_begin_of_line();
			break;
		case 'I':
			model_->to_begin_of_line();
			write_mode();
			break;
		case '$': case KEY_END:
			model_->to_end_of_line();
			break;
		case 'S':
			model_->to_end_of_line();
			write_mode();
			break;
		case 'A':
			model_->del_line();
			write_mode();
			break;
		case 'r':
			symbol = getch();
			model_->replace_sym(symbol);
			break;
		case ':':
			model_->cmd_.clear();
			command_mode();
			break;
		case '/':case '?':
			model_->search_word_.clear();
			search_mode(symbol);
			break;
		case KEY_LEFT:
			model_->cur_to_left();
			break;
		case KEY_RIGHT:
			model_->cur_to_right();
			break;
		case KEY_DOWN:
			model_->cur_to_down();
			break;
		case KEY_UP:
			model_->cur_to_up();
			break;
		case KEY_PPAGE:
			model_->page_up();
			break;
		case KEY_NPAGE:
			model_->page_down();
			break;
		case 'g':
			model_->cur_ctrl_home();
			break;
		case 'G':
			model_->cur_ctrl_end();
			break;
		case 'w':
			model_->cur_to_end_of_word();
			break;
		case 'b':
			model_->cur_to_begin_of_word();
			break;
		case 'x':
			model_->del_key();
			break;
		case 'p':
			model_->paste();
			break;
		case 'd': case 'y': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			model_->cmd_.clear();
			do {
				model_->cmd_.append(1, (char)symbol);
				symbol = getch();
			} while (symbol != '\n');
			navigation_commands();
			break;
		case 'n':
			model_->search_forward();
			break;
		case'N':
			model_->search_backward();
			break;
		default:
			break;
		}
	}
}

void Controller::write_mode() {
	chtype_adapted symbol = KEY_LEFT;
	model_->mode_ = "Insert mode";
	model_->view_->print_screen();
	while (symbol != KEY_HOME) {
		symbol = getch();
		switch (symbol) {
		case KEY_LEFT:
			model_->cur_to_left();
			break;
		case KEY_RIGHT:
			model_->cur_to_right();
			break;
		case KEY_DOWN:
			model_->cur_to_down();
			break;
		case KEY_UP:
			model_->cur_to_up();
			break;
		case KEY_PPAGE:
			model_->page_up();
			break;
		case KEY_NPAGE:
			model_->page_down();
			break;
		case '\x08': // KEY_BACKSPACE
			model_->backspace_key();
			break;
		case KEY_DC:
			model_->del_key();
			break;
		case KEY_IC:
			symbol = getch();
			model_->replace_sym(symbol);
			break;
		case KEY_HOME:
			break;
		default:
			model_->insert_sym(symbol);
			break;
		}
		model_->view_->print_screen();
	}
	navigation_mode();
}

void Controller::command_commands() {
	if (model_->cmd_[0] >= '0' && model_->cmd_[0] <= '9') {
		model_->go_to_line(atoi(model_->cmd_.c_str()));
	}
	else if (model_->cmd_ == "q!") {
		exit_program_ = true;
	}
	else if (model_->cmd_ == "q") {
		exit_program_ = true;
		model_->save_to_file();
	}
	else if (model_->cmd_[0] == 'o') {
		model_->cmd_.erase(0, 2);
		model_->open_file();
	}
	else if (model_->cmd_ == "x" || model_->cmd_ == "wq!") {
		model_->save_to_file();
	}
	else if (model_->cmd_ >= "w") {
		if (model_->cmd_ == "w") {
			model_->save_to_file();
		}
		else {
			model_->cmd_.erase(0, 2);
			model_->file_ = model_->cmd_;
			model_->save_to_new_file();
		}
	}
	else if (model_->cmd_ == "h") {
		model_->help();
		getch();
	}
	else if (model_->cmd_ =="set num") {
		PrintScreenLineNumbering* tmp = new PrintScreenLineNumbering(model_);
		tmp->execute();
		delete tmp;
	}
	else if (model_->cmd_ == "set nonum") {
		PrintScreenNoLineNumbering* tmp = new PrintScreenNoLineNumbering(model_);
		tmp->execute();
		delete tmp;
	}
}

void Controller::command_mode() {
	model_->mode_ = "Command mode";
	model_->view_->print_command_screen();
	chtype_adapted symbol = getch();
	while (symbol != KEY_HOME && symbol != '\n' ) {
		switch (symbol) {
		case '\b':
			model_->cmd_.erase(model_->cmd_.length() - 1, 1);
			model_->view_->print_command_screen();
			symbol = getch();
			break;
		default:
			model_->cmd_.append(1, (char)symbol);
			model_->view_->print_command_screen();
			symbol = getch();
			break;
		}
		model_->view_->print_screen();
	}
	if (symbol == '\n') {
		command_commands();
	}
	navigation_mode();
}

void Controller::search_mode(chtype_adapted search_mode) {
	model_->mode_ = "Search mode";
	model_->view_->print_search_screen(search_mode);
	chtype_adapted symbol = getch();
	while (symbol != KEY_HOME && symbol != '\n') { //if home then search_word just clears
		switch (symbol) {
		case '\b':
			model_->search_word_.erase(model_->search_word_.size() - 1, 1);
			model_->view_->print_search_screen(search_mode);
			symbol = getch();
			break;
		default:
			model_->search_word_.append(1, (char)symbol);
			model_->view_->print_search_screen(search_mode);
			symbol = getch();
			break;
		}
	}
	if (symbol == '\n') {
		if (search_mode == '/') {
			model_->search_forward();
		}
		else if (search_mode == '?') {
			model_->search_backward();
		}
	}
	navigation_mode();
}
