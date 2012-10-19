/* 
 * File:   NumberFormatException.h
 * Author: zeji
 *
 * Created on 22 June 2010, 00:25
 */

#ifndef _NUMBERFORMATEXCEPTION_H
#define	_NUMBERFORMATEXCEPTION_H

#include <exception>
#include <stdexcept>
using namespace std;

class NumberFormatException : public std::runtime_error
{
public:
    NumberFormatException(const string& info) : std::runtime_error(info)
    {
    }
};

#endif	/* _NUMBERFORMATEXCEPTION_H */

