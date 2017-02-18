#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <stdbool.h>

// forward declaration
typedef struct IniParser_S* IniParser_T;

// Construct Parser and parse given filename.
IniParser_T IniParser_Create ( const char* const filename );

// Destructor, release allocate resources
void IniParser_Destroy ( IniParser_T P );

// Return the result of Constructor, i.e., 0 on success.
int IniParser_CheckError ( IniParser_T P );

// Get a string value from INI file, returning default_value if not found.
const char* IniParser_Get ( IniParser_T P, 
                            const char* const section, 
                            const char* const name, 
                            const char* const default_value );

// Get an integer (long) value from INI file, returning default_value if
// not found or not a valid integer (decimal "1234", "-1234", or hex "0x4d2").
long IniParser_GetInteger ( IniParser_T P, 
                            const char* const section, 
                            const char* const name, 
                            const long default_value );

// Get a real (floating point double) value from INI file, returning
// default_value if not found or not a valid floating point value
// according to strtod().
double IniParser_GetDouble ( IniParser_T P, 
                             const char* const section, 
                             const char* const name, 
                             const double default_value );

// Get a boolean value from INI file, returning default_value if not found or if
// not a valid true/false value. Valid true values are "true", "yes", "on", "1",
// and valid false values are "false", "no", "off", "0" (not case sensitive).
bool IniParser_GetBoolean ( IniParser_T P,  
                            const char* const section, 
                            const char* const name, 
                            const bool default_value );


#endif /* INI_PARSER_H */