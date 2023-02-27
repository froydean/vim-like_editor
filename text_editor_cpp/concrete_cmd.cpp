#include "command.cpp"
#include "model.h"

class PrintScreenLineNumbering : public Command {
private:
	Model* pModel;
public:
	PrintScreenLineNumbering(Model* model){
		pModel = model;
	}
	void execute() override {
		pModel->set_line_numbering();
	}

};

class PrintScreenNoLineNumbering : public Command {
private:
	Model* pModel;
public:
	PrintScreenNoLineNumbering(Model* model) {
		pModel = model;
	}
	void execute() override {
		pModel->unset_line_numbering();
	}
};