#include "model.h"
#include "view.h"

Model::Model() {
	view_ = new View(this);
	file_ = "File";
	cmd_ = "";
	search_word_ = "";
	mode_ = "Navigation mode";
	cur_line_ = 0;
	cur_pos_ = 0;
	chars_in_line_count.resize(0x80000);
	last_line_num_ = 0;

	MyString buffer_ = "";
	MyString buffer_for_copying_ = "";
}

Model::~Model() {
	delete view_;
}

void Model::cur_to_left() {
	if (cur_pos_) { // > 0
		cur_pos_--;
	}
	else {
		if (cur_line_) { // if not first line
			cur_line_--;
			cur_pos_ = chars_in_line_count[cur_line_];
		}
	}
}

void Model::cur_to_right() {
	if (cur_pos_ < chars_in_line_count[cur_line_]) {
		cur_pos_++;
	}
	else {
		if (cur_line_ < last_line_num_) {
			cur_line_++;
			cur_pos_ = 0;
		}
	}
}

void Model::cur_to_up() {
	if (cur_line_) { // if not first line
		cur_line_--;
		if (cur_pos_ > chars_in_line_count[cur_line_]) {
			cur_pos_ = chars_in_line_count[cur_line_];
		}
	}
}

void Model::cur_to_down() {
	if (cur_line_ < last_line_num_) {
		cur_line_++;
		if (cur_pos_ > chars_in_line_count[cur_line_]) {
			cur_pos_ = chars_in_line_count[cur_line_];
		}
	}
}

void Model::page_up() {
	int tmp_line = cur_line_;
	tmp_line -= view_->max_y - 2;
	if (tmp_line <= 0)
		cur_line_ = 0;
	else
		cur_line_ = tmp_line;
	cur_pos_ = 0;
}

void Model::page_down() {
	cur_line_ += view_->max_y - 2;
	if (cur_line_ >= last_line_num_)
		cur_line_ = last_line_num_;
	cur_pos_ = 0;
}

void Model::backspace_key() {
	if (cur_pos_ == 0 && cur_line_ == 0)
		return;
	buffer_.erase(get_index_of_cur_pos()-1, 1);
	if (cur_pos_) {
		cur_pos_--;
		chars_in_line_count[cur_line_]--;
	}
	else {
		cur_line_--;
		cur_pos_ = chars_in_line_count[cur_line_];
		last_line_num_--;
	}
	update_lines();
}

void Model::del_key() {
	int tmp_pos = cur_pos_;
	int tmp_line = cur_line_;
	cur_to_right();
	if (tmp_pos != cur_pos_ || tmp_line != cur_line_)
		backspace_key();
}

void Model::insert_sym(chtype_adapted sym) {
	buffer_.insert(get_index_of_cur_pos(), 1, (char)sym);
	if (sym == '\n') {
		update_lines();
		cur_pos_ = 0;
		cur_line_++;
	}
	else {
		cur_pos_++;
		chars_in_line_count[cur_line_]++; //may be extra
	}
}

void Model::del_word() {
	int index = get_index_of_cur_pos() - 1;
	if (index+1 < buffer_.size() && (buffer_[index + 1] == '\n' || buffer_[index + 1] == ' ')) {
		return;
	}
	while (buffer_[index] != ' ' && buffer_[index] != '\n' && cur_pos_ >= 1) {
		index--;
		cur_to_left();
	}
	index++;
	while (buffer_[index] != ' ' && buffer_[index] != '\n' && index != buffer_.size()) {
		buffer_.erase(index, 1);
	}
	buffer_.erase(index, 1);
}

void Model::del_line() {
	to_end_of_line();
	while (chars_in_line_count[cur_line_] > 0 && cur_pos_ != 0)
		backspace_key();
}

void Model::copy_line() {
	buffer_for_copying_.clear();
	to_begin_of_line();
	int index = get_index_of_cur_pos();
	buffer_for_copying_.append(buffer_.c_str() + index, 0, chars_in_line_count[cur_line_]);
}

void Model::copy_word() {
	buffer_for_copying_.clear();
	int index = get_index_of_cur_pos() - 1;

	while (buffer_[index] != ' ' && buffer_[index] != '\n' && cur_pos_ >= 1) {
		index--;
		cur_to_left();
	}
	index++;
	while (buffer_[index] != ' ' && buffer_[index] != '\n' && cur_pos_ < chars_in_line_count[cur_line_]) {
		buffer_for_copying_.append(1, buffer_[index]);
		index++;
		cur_to_right();
	}
}

void Model::go_to_line(unsigned int num) {
	if (num > last_line_num_ || num < 0) return;
	cur_line_ = num;
	cur_pos_ = 0;
}

void Model::to_begin_of_line() {
	cur_pos_ = 0;
}

void Model::to_end_of_line() {
	cur_pos_ = chars_in_line_count[cur_line_];
}

void Model::replace_sym(chtype_adapted symbol) {
	del_key();
	insert_sym(symbol);
	cur_to_left();
}

void Model::cur_ctrl_home() {
	cur_pos_ = 0;
	cur_line_ = 0;
}

void Model::cur_ctrl_end() {
	cur_line_ = last_line_num_;
	cur_pos_ = chars_in_line_count[cur_line_];
}
	
void Model::cur_to_end_of_word() {
	unsigned int index = get_index_of_cur_pos();

	while ((buffer_[index +1] == ' ' || buffer_[index+1] == '\t' || buffer_[index+1] == '\n') && index+1 < buffer_.length()) {
		cur_to_right();
		index++;
	}
	while ((buffer_[index] == ' ' || buffer_[index] == '\t' || buffer_[index] == '\n') && index < buffer_.length()) {
		cur_to_right();
		index++;
	}
	while (!(buffer_[index] == ' ' || buffer_[index] == '\t' || buffer_[index] == '\n') && index < buffer_.length()) {
		cur_to_right();
		index++;
	}
	cur_to_left();
}

void Model::cur_to_begin_of_word() {
	
	int index = get_index_of_cur_pos();

	if ((buffer_[index - 1] == ' ' || buffer_[index - 1] == '\t' || buffer_[index - 1] == '\n') && index - 1 > 0) {
		cur_to_left();
		index--;
	}
	
	while ((buffer_[index] == ' ' || buffer_[index] == '\t' || buffer_[index] == '\n') && index > 0) {
		cur_to_left();
		index--;
	}
	while (!(buffer_[index] == ' ' || buffer_[index] == '\t' || buffer_[index] == '\n') && index > 0) {
		cur_to_left();
		index--;
	}
	cur_to_right();
}

void Model::paste() {
	if (!buffer_for_copying_.size())
		return;
	int len = buffer_for_copying_.size();
	for (int i = 0; i < len; i++) {
		insert_sym(buffer_for_copying_[i]);
	}
}

//only txt checked
void Model::open_file() {
	int len;
	file_ = cmd_;
	char* temp = copy_file_to_buffer(file_.c_str(), len);
	if (temp != nullptr) {
		buffer_.clear(); //all the things cleared?
		buffer_.append(temp);
		delete[]temp;
	}
	cur_ctrl_home();
	update_lines();
	cmd_.clear();
	search_word_.clear();
}

void Model::save_to_file() {
	ofstream f;
	f.open(file_.c_str(), ios_base::out | ios_base::trunc);
	if (!f.is_open() || !file_.length())
		return;
	f << buffer_.c_str();
}

void Model::save_to_new_file() {
	ofstream f;
	f.open(cmd_.c_str(), ios_base::out | ios_base::trunc);
	if (!f.is_open() || !strlen(cmd_.c_str()))
		return;
	f << buffer_.c_str();
}

void Model::search_forward() {
	int size = search_word_.size();
	if (!size)
		return;
	int index = get_index_of_cur_pos();
	//if not enough plce to search forward
	if (buffer_.size() - index < size) {
		return;
	}
	size_t found_ind = buffer_.find(search_word_.c_str(), index);
	
	// if found
	if (found_ind != -1) {
		for (int i = index; i < found_ind; i++) {
			if (buffer_[i] == '\n') {
				cur_line_++;
				cur_pos_ = 0;
			}
			else {
				cur_pos_++;
			}
		}
	}
	else {
		//won't display cause nav mode immidiately
		view_->print_not_found();
	}
}

void Model::search_backward() {
	int size = search_word_.size();
	if (!size)
		return;
	int index = get_index_of_cur_pos() - 1; //inf found at at te index it's not quite correct
	//if not enogh place to search backwards
	if (index < size) {
		return;
	}
	//could be better idgf
	//can involve word under index
	vector<size_t>searches_;
	searches_.resize(0x800);

	int j = 0;
	string search_word = search_word_.c_str();
	for (int i = 0; i < index + 1 - size;) {
		size_t tmp = buffer_.find(search_word, i);
		if (tmp > index + 1 - size) { //out of range of search
			break;
		}
		if (tmp == -1) { //not found
			searches_[j] = tmp;
			break;
		}
		else {
			searches_[j] = tmp;
			i = tmp+size;
			j++;
		}
	}

	size_t found_ind = -1;
	if (j != 0 && searches_[j-1] != 0) {
		j--;
		found_ind = searches_[j];
	}
		
	cur_pos_ = 0;
	cur_line_ = 0;
	if (found_ind != -1) {
		for (int i = 0; i < found_ind; i++) {
			if (buffer_[i] == '\n') {
				cur_line_++;
				cur_pos_ = 0;
			}
			else {
				cur_pos_++;
			}
		}
	}
	else {
		//won't display cause nav mode immidiately
		view_->print_not_found();
	}
}

void Model::help() {
	view_->print_help();
}

void Model::set_line_numbering() {
	view_->line_numbering_ = true;
}

void Model::unset_line_numbering() {
	view_->line_numbering_ = false;
}

char* Model::copy_file_to_buffer(const char* name, int& len) {
	FILE* file;
	fopen_s(&file, name, "r");
	if (file != NULL) {
		fseek(file, 0, SEEK_END);
		unsigned long size = ftell(file);
		fseek(file, 0, SEEK_SET);
		char* temp = new char[size + 1] {0};
		temp[fread(temp, sizeof(char), size, file)] = 0;
		len = size;
		fclose(file);
		return temp;
	}
	else return nullptr;
}

void Model::update_lines() {
	for (int i = 0; i <= last_line_num_; i++)
		chars_in_line_count[i] = 0;
	int i = 0, line = 0, len = buffer_.length();
	while (i < len) {
		if (buffer_[i] == '\n') {
			line++;
		}
		else {
			chars_in_line_count[line]++;
		}
		i++;
	}
	last_line_num_ = line;
}

unsigned int Model::get_index_of_cur_pos() {
	int iter = 0;
	for (int i = 0; i < cur_line_; ++i)
		iter += chars_in_line_count[i] + 1;
	iter += cur_pos_;
	return iter;
}
