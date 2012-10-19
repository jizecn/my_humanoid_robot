/* 
 * File:   FileFormatException.h
 * Author: ze
 *
 * Created on 22 June 2010, 18:46
 */

#ifndef _FILEFORMATEXCEPTION_H
#define	_FILEFORMATEXCEPTION_H


#include <exception>
#include <stdexcept>
using namespace std;

class FileFormatException : public std::runtime_error
{
public:
    FileFormatException(const string& info) : std::runtime_error(info)
    {
    }
};

#endif	/* _FILEFORMATEXCEPTION_H */

