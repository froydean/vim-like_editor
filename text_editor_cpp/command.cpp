class Command {
private:
public:
	virtual ~Command(){
	}
	virtual void execute() = 0;
};