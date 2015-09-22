#include "commonDef.h"


#ifndef __DEBUG
 #define assert(ignore)  ((void)0)
 #else
 #undef assert
 #undef __myassert
 #define assert(expression)  \
  ((void)((expression) ? 0 : (__myassert ( __FUNCTION__, __LINE__), 0)))

 #define __myassert( file, line)  \
  __myassfail("FA at line %d Funk `%s'.\n\r",    \
         line, file)

 static void
 __myassfail(const char *format,...)
 {
    va_list arg;
    static char mystderr[89];
    char s[89];
    va_start(arg, format);
    (void)vsprintf(&mystderr[0], format, arg);
    memcpy(s,&mystderr[0],89);
    output_serial(s, 89);
    #if defined (SIMULATION)
    printf("%s\n", s);
    #endif
    while(1);
    va_end(arg);
 }
 #endif