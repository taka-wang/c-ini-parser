#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

// forward declaration
typedef struct IniParser_S *IniParserPtr_T;

// Construct Parser and parse given ini filename.
IniParserPtr_T IniParser_Create ( const char *const ini_filename );

// Destructor, release allocate resources
void IniParser_Destroy ( IniParserPtr_T P );

// Return the result of Constructor, i.e., 0 on success.
int16_t IniParser_CheckInstanceError ( IniParserPtr_T P );

// Get a string value from INI file, returning default_value if not found.
const char *IniParser_GetString (
    IniParserPtr_T P, const char *const section, const char *const name,
    const char *const default_value );

// Get an integer (long) value from INI file, returning default_value if
// not found or not a valid integer (decimal "1234", "-1234", or hex "0x4d2").
int32_t IniParser_GetInteger (
    IniParserPtr_T P, const char *const section, const char *const name,
    const int32_t default_value );

// Get a real (floating point double) value from INI file, returning
// default_value if not found or not a valid floating point value
// according to strtod().
double IniParser_GetDouble (
    IniParserPtr_T P, const char *const section, const char *const name,
    const double default_value );

// Get a boolean value from INI file, returning default_value if not found or if
// not a valid true/false value. Valid true values are "true", "yes", "on", "1",
// and valid false values are "false", "no", "off", "0" (not case sensitive).
bool IniParser_GetBoolean (
    IniParserPtr_T P, const char *const section, const char *const name,
    const bool default_value );
