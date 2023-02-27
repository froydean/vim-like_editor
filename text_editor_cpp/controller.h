#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "curses.h"
#include "model.h"
#include "view.h"
#include "concrete_cmd.cpp"

class Controller {

private:
	bool exit_program_;
	Model* model_;

	void navigation_commands();
	void write_mode();
	void command_commands();
	void command_mode();
	void search_mode(chtype_adapted search_mode);

public:

	Controller();
	~Controller();

	void navigation_mode();
};

#endif //CONTROLLER_H