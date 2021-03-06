#ifndef FILEINPUTOUTPUTSTREAM_H
#define FILEINPUTOUTPUTSTREAM_H

#include "inputoutputstream.h"
#include "filesystemdefs.h"
#include <string>

using namespace std;

class FileInputOutputStream: public InputOutputStream {
    public:
        FileInputOutputStream(const FileInputOutputStream& other);
        FileInputOutputStream(const std::string& fileName, const char* flags);
        virtual ~FileInputOutputStream();

        virtual unsigned char readChar();
		  /* Reads 1 bytes in the input (little endian order) */
		  virtual bool readBoolean ();
		  /* Reads 2 bytes in the input (little endian order) */
		  virtual __int16 readShortInt ();
		  /* Reads 4 bytes in the input (little endian order) */
		  virtual __int32 readInt ();
		  /* Reads 8 bytes in the input (little endian order) */
		  virtual __int64 readLong ();
		  /* Reads a 16 byte long in the input */
		  virtual __int64 readLong64();
		  /* Reads a 4 byte float in the input */
		  virtual float readFloatIEEE ();
		  /* Reads a 8 byte double in the input */
		  virtual double readDoubleIEEE ();
		  /* Read a chars */
		  virtual char* readChars();
		  virtual char* readChars(__int32 length);
		  virtual const char* readFull();

		  virtual std::string* readString();

		  virtual void writeChar (unsigned char v);
		  /* Write 1 bytes in the output (little endian order) */
		  virtual void writeBoolean (bool v);
		  /* Write 2 bytes in the output (little endian order) */
		  virtual void writeShortInt (__int16 v);
		  /* Write 4 bytes in the output (little endian order) */
		  virtual void writeInt (__int32 v);
		  /* Write 4 bytes in the output (little endian order) */
		  virtual void writeLong (__int64 v);
		  /* Write a 4 byte float in the output */
		  virtual void writeFloatIEEE (float v);
		  /* Write a 8 byte double in the output */
		  virtual void writeDoubleIEEE (double v);
		  /* Write a char */
		  virtual void writeChars(const char* text, __int32 len);
		  virtual void writeString(const std::string& text);

		  virtual void seek(__int64 pos, SEEK_DIRECTION direction = FROMSTART_SEEK);
		  virtual __int64 currentPos() const;

		  virtual const std::string fileName() const;
		  virtual bool eof();
		  virtual void close();

		  virtual void flush();

		  virtual bool isClosed();

	 private:
		  void write(char* buffer, int len);
		  __int64 read(char* buffer, __int32 len); 

	 private:
		  std::string _fileName;
#ifndef A
		  FILE* _pFile;
#else
		  HANDLE _pFile;
		  bool _eof;
#endif
		  bool _open;
};

#endif // FILEINPUTOUTPUTSTREAM_H
