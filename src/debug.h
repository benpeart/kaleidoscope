#ifndef DEBUG_H
#define DEBUG_H

/* Control whether debugging macros are active at compile time */
#undef DB_ACTIVE
#ifdef DEBUG
#define DB_ACTIVE 1
#else
#define DB_ACTIVE 0
#endif /* DEBUG */

/*
** Usage:  DB_PRINT((...));
** Usage:  DB_PRINTLN((...));
**
** "..." is whatever extra arguments fmt requires (possibly nothing).
**
** The structure of the macros means that the code is always validated
** but is not called when DEBUG is undefined.
** -- See chapter 8 of 'The Practice of Programming', by Kernighan and Pike.
*/
#define DB_PRINT(...)\
            do { if (DB_ACTIVE) Serial.print(__VA_ARGS__); } while (0)
#define DB_PRINTF(...)\
            do { if (DB_ACTIVE) Serial.printf(__VA_ARGS__); } while (0)
#define DB_PRINTLN(...)\
            do { if (DB_ACTIVE) Serial.println(__VA_ARGS__); } while (0)

#endif // DEBUG_H