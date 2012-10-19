/* 
 * File:   FileIOException.h
 * Author: zeji
 *
 * Created on 21 June 2010, 07:08
 */

#ifndef _FILEIOEXCEPTION_H
#define	_FILEIOEXCEPTION_H

#include <exception>
#include <stdexcept>
using namespace std;

class FileIOException : public std::runtime_error
{
public:
    FileIOException(const string& info) : std::runtime_error(info)
    {
    }
};

#endif	/* _FILEIOEXCEPTION_H */

