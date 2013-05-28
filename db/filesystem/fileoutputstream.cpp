// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
// 
// This file is part of the djondb project, for license information please refer to the LICENSE file,
// the application and libraries are provided as-is and free of use under the terms explained in the file LICENSE
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

#include "fileoutputstream.h"
#include "util.h"
#include <string.h>
#include <cstdio>
#include <iostream>
#include <boost/crc.hpp>
#include <limits.h>

FileOutputStream::FileOutputStream(char* fileName, const char* flags) {
    _pFile = fopen(fileName, flags);
    _fileName = fileName;
}

FileOutputStream::~FileOutputStream() {
    close();
}

bool FileOutputStream::isOpen() const {
	return (_pFile != NULL);
}

/* Write 1 byte in the output */
void FileOutputStream::writeChar (unsigned char v)
{
    fwrite(&v, 1, 1, _pFile);
}

/* Write 1 bytes in the output (little endian order) */
void FileOutputStream::writeBoolean (bool v)
{
	writeData<char>((char)v);
}

/* Write 2 bytes in the output (little endian order) */
void FileOutputStream::writeShortInt (__int16 v)
{
	writeData<__int16>(v);
}

/* Write 4 bytes in the output (little endian order) */
void FileOutputStream::writeInt (__int32 v)
{
	writeData<__int32>(v);
}

/* Write 4 bytes in the output (little endian order) */
void FileOutputStream::writeLong (__int64 v)
{
	writeData<__int64>(v);
}

/* Write 8 bytes in the output (little endian order) */
void FileOutputStream::writeLong64 (__LONG64 v)
{
	writeData<__LONG64>(v);
}

/* Write a 4 byte float in the output */
void FileOutputStream::writeFloatIEEE (float v)
{
	fwrite(&v, 1, sizeof(v), _pFile);
}

/* Write a 8 byte double in the output */
void FileOutputStream::writeDoubleIEEE (double v)
{
	fwrite(&v, 1, sizeof(v), _pFile);
}

void FileOutputStream::writeChars(const char *text, __int32 len) {
	writeInt(len);
	fwrite(text, 1, len, _pFile);
}

void FileOutputStream::writeString(const std::string& text) {
	const char* c = text.c_str();
	__int32 l = strlen(c);
	writeChars(c, l);
}

__int64 FileOutputStream::crc32(__int32 pos) {
	fflush(_pFile);
	__int64 originalPos = currentPos();
	fseek(_pFile, 0, SEEK_END);
	__int64 bufferSize = currentPos();
	bufferSize -= pos;
	seek(pos);

	char* buffer = (char*)malloc(bufferSize+1);
	memset(buffer, 0, bufferSize + 1);
	fread(buffer, 1, bufferSize, _pFile);

	boost::crc_32_type crc;
	crc.process_bytes(buffer, bufferSize);
	__int64 result = crc.checksum();
	std::cout << "CRC: Output" << result << std::endl;

	// back to the original position
	seek(originalPos);
	free(buffer);
	return result;
}

void FileOutputStream::seek(__int64 i, SEEK_DIRECTION direction) {
	fflush(_pFile);
	if (direction == FROMSTART_SEEK) {
		fseek (_pFile, i, SEEK_SET);
	} else {
		fseek (_pFile, i, SEEK_END);
	}
}

__int64 FileOutputStream::currentPos() const {
	return ftell(_pFile);
}

void FileOutputStream::close() {
	if (_pFile) {
		flush();
		fclose(_pFile);
		_pFile = 0;
	}
}

void FileOutputStream::flush() {
	fflush(_pFile);
}

const char* FileOutputStream::fileName() const {
	return _fileName;
}
