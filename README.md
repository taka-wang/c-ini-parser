# C-INI-Parser

[![Build Status](https://travis-ci.org/taka-wang/c-ini-parser.svg?branch=master)](https://travis-ci.org/taka-wang/c-ini-parser)
[![GitHub license](https://img.shields.io/badge/license-BSD-blue.svg)](https://raw.githubusercontent.com/taka-wang/c-ini-parser/master/LICENSE)

An simple INI parser based on [inih](https://github.com/benhoyt/inih) in OOP style C99.

Preserve parsed INI data in hashtable and provide a key-value access interface.

## // C API

```c

// constructor
IniParserPtr_T IniParser_Create ( const char* const ini_filename );

// destructor
void IniParser_Destroy ( IniParserPtr_T P );

// check error
int16_t IniParser_CheckInstanceError ( IniParserPtr_T P );

// get string via key string
const char* const IniParser_GetString ( IniParserPtr_T P, const char* const section, const char* const name, const char* const default_value );

// get integer via key string
int32_t IniParser_GetInteger ( IniParserPtr_T P, const char* const section, const char* const name, long default_value );

// get real(double) via key string
double IniParser_GetDouble ( IniParserPtr_T P, const char* const section, const char* const name, double default_value );

// get boolean via key string
bool IniParser_GetBoolean ( IniParserPtr_T P, const char* const section, const char* const name, bool default_value );
```

## // Example

### Build from example folder via gcc (link iniparser.c and ini.c)

```bash
gcc -std=c99 -Wall -o test test.c ../src/iniparser.c ../src/internal/ini.c -I../src
```

### Build via CMake

- `DENABLE_EXAMPLE`: build examples flag, default is `TRUE`.
- `DENABLE_SHAREDLIB`: share or static library flag, default is `TRUE` (shared).

```bash
mkdir build && cd build
cmake .. [-DENABLE_EXAMPLE=TRUE -DENABLE_SHAREDLIB=FALSE]
make
make install
```

### Sample INI taken from [inih repo](https://github.com/benhoyt/inih)

```bash
; Just comments
; Another comments

[protocol]             ; Protocol configuration, this is section
version=6              ; IPv6, this is name


[user]
name = Bob Smith       ; Spaces around '=' are stripped
email = bob@smith.com  ; And comments (like this) ignored
active = true          ; Test a boolean
pi = 3.14159           ; Test a floating point number
```

### Sample Code

```c
#include <stdio.h>
#include "iniparser.h"

int main(void)
{
    IniParserPtr_T p = IniParser_Create ("test.ini");
    printf ("Is error: %" PRId16 "\n", IniParser_CheckInstanceError(p));

    const char * v1 = IniParser_GetString (p, "user", "name", "taka");
    printf ("user-name: %s\n", v1);

    int32_t v2 = IniParser_GetInteger (p, "protocol", "version", 4);
    printf ("protocol-version: %" PRId32 "\n", v2);

    double v3 = IniParser_GetDouble (p, "user", "pi", 10.2);
    printf ("user-pi: %f\n", v3);

    bool v4 = IniParser_GetBoolean (p, "user", "active", false);
    printf ("user-active: %s\n", v4 ? "true" : "false");

    IniParser_Destroy(p);

    return 0;
}
```

---

## // Dependency

- [inih](https://github.com/benhoyt/inih): Simple .INI file parser in C
- [uthash](https://github.com/troydhanson/uthash): C macros for hash tables

## // License

BSD

## // Acknowledgments

The design of this library was inspired by [inih](https://github.com/benhoyt/inih), [libzdb](https://github.com/taka-wang/libzdb) and [How to C in 2016](https://matt.sh/howto-c).