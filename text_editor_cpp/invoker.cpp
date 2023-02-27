#include "concrete_cmd.cpp"

class Switch {
private:
    Command* lineNumOn_;
    Command* lineNumOff_;

public:

    Switch() {

    }

    ~Switch() {
        delete lineNumOn_;
        delete lineNumOff_;
    }

    void setLineNumOn(Command* cmd) {
        lineNumOn_ = cmd;
    }

    void setLineNumOff(Command* cmd) {
        lineNumOff_ = cmd;
    }

    void lineNumOn() {
        lineNumOn_->execute();
    }

    void lineNumOff() {
        lineNumOff_->execute();
    }
};