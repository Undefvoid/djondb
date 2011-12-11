#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

#include <string>

class OutputStream
{
public:
    virtual void writeChar (unsigned char v) = 0;
    /* Write 2 bytes in the output (little endian order) */
    virtual void writeInt (int v) = 0;
    /* Write 4 bytes in the output (little endian order) */
    virtual void writeLong (long v) = 0;
    /* Write a 4 byte float in the output */
    virtual void writeFloatIEEE (float v) = 0;
    /* Write a 8 byte double in the output */
    virtual void writeDoubleIEEE (double v) = 0;
    /* Write a char */
    virtual void writeChars(const char* text, int len) = 0;
    virtual void writeString(const std::string& text) = 0;

};

#endif // OUTPUTSTREAM_H
