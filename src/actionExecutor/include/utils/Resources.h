#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#if defined __linux__
    #define FILESEPARATOR "/"
#elif defined __linux
    #define FILESEPARATOR "/"
#elif defined __WIN32__
    // Windows stuff
    #define FILESEPARATOR "\\"
#else
    // other stuff
    #define FILESEPARATOR "/"
#endif

namespace KasparFile {
    enum RetCode {
	    OK, 
        FileFormatError, 
        FileIOError, 
        NumberFormatError
    };
}

#endif // _RESOURCES_H_