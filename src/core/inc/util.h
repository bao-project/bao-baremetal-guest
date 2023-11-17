#ifndef UTIL_H
#define UTIL_H

/**
 * Check if a given macro was defined. Note it only works wither if the macro is undefined or
 * defined to the value 1. If the macro is defined with any other value it will fail recognizing
 * its defined.
 */
#define DEFINED(MACRO)                  _DEFINED(MACRO)
#define _DEFINED_1                      0,
#define _DEFINED(VALUE)                 __DEFINED(_DEFINED_##VALUE)
#define __DEFINED(VALUE)                ___DEFINED(VALUE true, false)
#define ___DEFINED(IGNORE, RESULT, ...) (RESULT)


#endif /* UTIL_H */
