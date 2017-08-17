#ifndef _IOSTREAM_H
#define _IOSTREAM_H

#ifndef _IOSTREAM_
#include <iostream>
#endif // !_IOSTREAM_

#ifdef UNICODE
#define FCout wcout
#define FCin wcin
#else
#define FCout cout
#define FCin  cin
#endif // UNICODE


#endif // !_IOSTREAM_H
