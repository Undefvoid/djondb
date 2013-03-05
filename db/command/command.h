#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <string>

using namespace std;

class BSONObj;
class Controller;
class OutputStream;
class InputStream;

enum COMMANDTYPE {
    INSERT,
    UPDATE,
    FIND,
    CLOSECONNECTION,
	 DROPNAMESPACE,
    SHUTDOWN,
	 SHOWDBS,
	 SHOWNAMESPACES,
	 REMOVE,
	 COMMIT,
	 ROLLBACK
};

class Command {
    public:
        Command(COMMANDTYPE commandType);

        Command(const Command& orig);
        virtual ~Command();

        virtual void execute() = 0;
        virtual void* result() = 0;

        virtual void writeCommand(OutputStream* out) const = 0;
        virtual void writeResult(OutputStream* out) const = 0;
        virtual void readResult(InputStream* is) = 0;

        Controller* dbController() const {
            return  _dbController;
        }

        void setDBController(Controller* dbController) {
            _dbController = dbController;
        }

        COMMANDTYPE commandType() const;
		  int resultCode() const {
			  return _resultCode;
		  }

		  std::string resultMessage() const {
			  return _resultMessage;
		  }

		  void setOptions(const BSONObj* options);

		  const BSONObj* options() const {
			  return _options;
		  }

	 protected:
		  void setResultCode(int rc) {
			  _resultCode = rc;
		  }

		  void setResultMessage(std::string message) {
			  _resultMessage = message;
		  }

		  int _resultCode;
		  std::string _resultMessage;

	 private:
		  COMMANDTYPE _commandType;
		  Controller* _dbController;
		  BSONObj* _options;
};

class CloseCommand: public Command {
	public:
		CloseCommand();

		CloseCommand(const CloseCommand& orig);
		virtual ~CloseCommand();

		virtual void execute();

		virtual void* result() {
			return NULL;
		}

		virtual void writeCommand(OutputStream* out) const;
		virtual void writeResult(OutputStream* out) const;
		virtual void readResult(InputStream* is);
};



#endif // COMMAND_H_INCLUDED
