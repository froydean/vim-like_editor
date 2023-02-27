//This file has MyString class and all funcs declaration
#ifndef MY_STRING_H
#define MY_STRING_H
#pragma warning (disable:4996)
#include <iostream>
#include <initializer_list>
#include <climits>
using namespace std;

class MyString {

private:

	char* head_string;
	size_t string_length;

	void set_fields_to_zero();
	bool is_max_input(const size_t count);
	void holy_trinity_init(const char* str, const size_t len);
	void clear_inner();

public:

	MyString();
	MyString(const char* user_string);
	MyString(const initializer_list <char> str_as_list);
	MyString(const string user_string);
	MyString(const char* user_string, const size_t count);
	MyString(const size_t count, const char one_char);
	MyString(const MyString & other);

	~MyString();

	MyString operator+(const MyString & other) const;
	MyString operator+(const char* user_string) const;
	MyString operator+(const string user_string) const;
	void operator+=(const char* user_string);
	void operator+=(const string user_string);
	void operator=(const char* user_string);
	void operator=(const string user_string);
	void operator=(const char sym);
	void operator=(const MyString & other);
	char operator[](const size_t index) const;
	bool operator>(const MyString & other)const;
	bool operator<(const MyString & other) const;
	bool operator>=(const MyString & other) const;
	bool operator<=(const MyString & other) const;
	bool operator!=(const MyString & other) const;
	bool operator==(const MyString & other) const;
	
	char* c_str() const;
	char* data() const;
	size_t length() const;
	size_t size() const;
	bool empty() const;
	size_t capacity() const;
	
	void shrink_to_fit();
	void clear();

	friend ostream& operator<<(ostream& out, const MyString & current_obj);
	friend istream& operator>>(istream& in, MyString & current_obj);

	void insert(const size_t index, const size_t count, const char sym);
	void insert(const size_t index, const char* user_string);
	void insert(const size_t index, const char* user_string, const size_t count);
	void insert(const size_t index, const string user_string);
	void insert(const size_t index, const string user_string, const size_t count);

	void erase(const size_t index, const size_t count);

	void append(const size_t count, const char sym);
	void append(const char* user_string);
	void append(const char* user_string, const size_t index, const size_t count);
	void append(const string user_string);
	void append(const string user_string, const size_t index, const size_t count);

	void replace(const size_t index, const size_t count, const char* user_string);
	void replace(const size_t index, const size_t count, const string user_string);

	MyString substr(const size_t index) const;
	MyString substr(const size_t index, const size_t count) const;

	size_t find(const char* user_string) const;
	size_t find(const char* user_string, const size_t index) const;
	size_t find(const string user_string) const;
	size_t find(const string user_string, const size_t index) const;
	
};

void run_tests_for_constructors();
void char_array_test();
void init_list_test();
void std_string_test();
void char_n_array_test();
void n_chars_test();
void copy_constr_test();

void run_tests_for_operators();
void opertr_plus();
void opertr_plus_equal();
void opertr_equal();
void opertr_ind();
void opertr_as();

void run_single_tests();

void run_insert_tests();

void run_erase_tests();

void run_append_tests();

void run_replace_tests();

void run_substr_tests();

void run_find_tests();

#endif //MY_STRING_H