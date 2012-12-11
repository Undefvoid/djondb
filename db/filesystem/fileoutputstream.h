#ifndef FILEOUTPUTSTREAM_H
#define FILEOUTPUTSTREAM_H

#include "outputstream.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

class FileOutputStream: public OutputStream
{
    public:
        FileOutputStream(char* fileName, const char* flags);
        virtual ~FileOutputStream();
    public:
        virtual void writeChar (unsigned char v);
        /* Write 2 bytes in the output (little endian order) */
        virtual void writeShortInt (short int v);
        /* Write 4 bytes in the output (little endian order) */
        virtual void writeInt (__int32 v);
        /* Write 4 bytes in the output (little endian order) */
        virtual void writeLong (__int64 v);
        /* Write 8 bytes in the output (little endian order) */
        virtual void writeLong64 (__LONG64 v);
        /* Write a 4 byte float in the output */
        virtual void writeFloatIEEE (float v);
        /* Write a 8 byte double in the output */
        virtual void writeDoubleIEEE (double v);
        /* Write a char */
        virtual void writeChars(const char* text, __int32 len);
        virtual void writeString(const std::string& text);

        virtual void seek(__int64);
        virtual __int64 currentPos() const;

        virtual __int64 crc32(__int32 pos);

        virtual void close();

        virtual void flush();
        virtual const char* fileName() const;

		  bool isOpen() const;

    private:
        char* _fileName;
        FILE* _pFile;
};

#endif // FILEOUTPUTSTREAM_H
