#ifndef _UTILS_H_
#define _UTILS_H_

#include <sstream>

#include <stdio.h>
#include <string.h>
//#include <string>
using namespace std;

/*
static int zjConvertToInt(char* str) {
int t = 0;
int i = sscanf(str, "%d", &t);
//printf(">>> %d\n", t);
if (i == EOF)
throw NumberFormatException("Cannot convert string into int, string contains non-numeric characters");
return t;
}*/

/*
  static int zjConvertToInt(char* str, int &t) {
  //int t = 0;
  int i = sscanf(str, "%d", &t);
  //printf(">>> %d\n", t);
  if (i == EOF)
  {
  return -1; // error .. as QT does not support exception, so return a value as C    
  //throw NumberFormatException("Cannot convert string into int, string contains non-numeric characters");
  }
  return t;
  }
*/

static bool zjConvertToInt(char* str, int &t) {
  //int t = 0;
  int i = sscanf(str, "%d", &t);
  return i != EOF;
}

static int zjStripNewline( char *str )
{
  //http://www.codeguru.com/forum/showthread.php?t=253826
  if(!str) return -1;
  for(int i = 0; i < strlen(str); i++)  {
    // assuming the last one or two chars are '\n' or '\r\n'
    if(str[i] == '\r' || str[i] == '\n')  {
      str[i] = '\0';
    }
  }
  return 0;
}

static void zjTrimString( string& str, const string &trimDel)
{
  // Trim Both leading and trailing spaces
  size_t startpos = str.find_first_not_of(trimDel); // Find the first character position after excluding leading blank spaces
  size_t endpos = str.find_last_not_of(trimDel); // Find the first character position from reverse af
  
  // if all spaces or empty return an empty string
  if(( string::npos == startpos ) || ( string::npos == endpos)) {
    str = "";
  }
  else
    str = str.substr( startpos, endpos-startpos+1 );
}

template <class T>
std::string to_string (const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}

#endif //_UTILS_H_
