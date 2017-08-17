#ifndef _STRING_H
#define _STRING_H

//C runtime Header Files
#ifndef _TCHAR_INC_
#include <tchar.h>
#endif // !_TCHAR_INC_

#ifndef _STRING_
#include <string>
#endif // !_STRING_

#ifdef UNICODE
#define FString wstring
#define TOSTRING(value) std::to_wstring(value)
#else
#define FString string
#define TOSTRING(value) std::to_string(value)
#endif // UNICODE

#endif // !_STRING_H
