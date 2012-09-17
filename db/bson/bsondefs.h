#ifndef BSON_DEFS_H_INCLUDED
#define BSON_DEFS_H_INCLUDED

#include <string>

typedef std::string t_keytype;


enum BSONTYPE {
    INT_TYPE,
    DOUBLE_TYPE,
    LONG_TYPE,
	 PTRCHAR_TYPE,
    STRING_TYPE,
    BSON_TYPE,
	 BSONARRAY_TYPE,
    UNKNOWN_TYPE,
	 NULL_TYPE
//    PTR
};

class BSONParseException: public std::exception {
	public:
		BSONParseException(const char* error) {
			_errorMessage = error;
		}

		BSONParseException(const BSONParseException& orig) {
			this->_errorMessage = orig._errorMessage;
		}

		virtual const char* what() const throw() {
			return _errorMessage;
		}

	private:
		const char* _errorMessage;
};

#endif //BSON_DEFS_H_INCLUDED
