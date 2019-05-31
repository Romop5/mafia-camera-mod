#include <stdio.h>
#define _snprintf snprintf

__declspec(dllexport) extern "C" int __snprintf(char* s, size_t n,
    const char* format, ...)
{
    return 0;
}
