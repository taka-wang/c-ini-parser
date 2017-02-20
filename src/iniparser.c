#include "iniparser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // strcasecmp

#include "internal/ini.h"
#include "internal/uthash.h"

#define STREQ( s1, s2 ) ( strcasecmp ( ( s1 ), ( s2 ) ) == 0 )
#define ALLOC( n ) calloc ( 1, n )
#define T IniParserPtr_T // IniParser_S pointer (Parser class)
#define KV KVPtr_T       // KV_S pointer (key value structure for hashtable)

enum                     // uname enum
{
    max_key_len = 100,   // max key length for hashtable
    max_value_len = 255  // max value length for hashtable
};

// pointer to internal key value item struct
typedef struct KV_S *KVPtr_T;

// INI parser structure
struct IniParser_S
{
    KVPtr_T hashtable; // hashtable root
    int16_t status;    // ini_parse result
};

// internal key value item structure for hashtable
struct KV_S
{
    char key[max_key_len];
    char value[max_value_len];
    UT_hash_handle hh; // makes this structure hashable
};

// ======================================
// private methods section
// ======================================

// concat_section_with_name concat section and name string as hash key
static void concat_section_with_name (
    const char *const section, const char *const name, char *key_arr,
    size_t key_arr_len )
{
    // concat section and name with @@ symbol
    snprintf ( key_arr, key_arr_len, "%s@@%s", section, name );
}

// hash_add insert key value to hash table
static void hash_add ( T P, const char *const key, const char *const value )
{
    KV kv_ptr = NULL;
    kv_ptr = (KV)ALLOC ( sizeof ( struct KV_S ) );
    if ( kv_ptr )
    {
        strncpy ( kv_ptr->key, key, strlen ( key ) );
        strncpy ( kv_ptr->value, value, strlen ( value ) );
        HASH_ADD_STR ( P->hashtable, key, kv_ptr );
    }
}

// hash_get_value get value from hashtable via key,
//  return char pointer is maintained by hashtable
static char *hash_get_value ( T P, const char *const key )
{
    KV kv_ptr = NULL;
    if ( P->hashtable )
    {
        HASH_FIND_STR ( P->hashtable, key, kv_ptr );
        return ( NULL != kv_ptr ) ? kv_ptr->value : "";
    }
    return "";
}

// ini parser handler
static int my_ini_handler (
    void *user, const char *section, const char *name, const char *value )
{
    assert ( user );
    T P = (T)user;
    char key[max_key_len];
    concat_section_with_name ( section, name, key, max_key_len );
    hash_add ( P, key, value );
    return 1;
}

// ======================================
// public methods section
// ======================================

T IniParser_Create ( const char *const ini_filename )
{
    T P = NULL;
    // compact form:
    //  if ( NULL == (P = (T)ALLOC ( sizeof ( struct IniParser_S ) ))) return NULL;
    P = (T)ALLOC ( sizeof ( struct IniParser_S ) );
    if ( NULL == P )
    {
        return NULL;
    }
    
    // instantiate hash table to NULL, important!
    P->hashtable = NULL;
    P->status = ini_parse ( ini_filename, my_ini_handler, P );
    return P;
}

void IniParser_Destroy ( T P )
{
    assert ( P );
    HASH_CLEAR ( hh, P->hashtable ); // empty hashtable

    free ( P->hashtable );
    P->hashtable = NULL;

    free ( P );
    P = NULL;
}

int16_t IniParser_CheckInstanceError ( T P )
{
    assert ( P );
    return P->status;
}

const char *IniParser_GetString (
    T P, const char *const section, const char *const name,
    const char *const default_value )
{
    assert ( P );
    char key[max_key_len];
    concat_section_with_name ( section, name, key, max_key_len );
    char *val_str = hash_get_value ( P, key );
    return strlen ( val_str ) > 0 ? val_str : default_value;
}

int32_t IniParser_GetInteger (
    T P, const char *const section, const char *const name,
    const int32_t default_value )
{
    assert ( P );
    const char *val_str = IniParser_GetString ( P, section, name, "" );
    char *end;
    int32_t val_long = strtol ( val_str, &end, 0 ); // base 0
    return end > val_str ? val_long : default_value;
}

double IniParser_GetDouble (
    T P, const char *const section, const char *const name,
    const double default_value )
{
    assert ( P );
    const char *val_str = IniParser_GetString ( P, section, name, "" );
    char *end;
    double val_double = strtod ( val_str, &end );
    return end > val_str ? val_double : default_value;
}

// bool pais structure
typedef struct pair_s
{
    const char *val_str;
    bool val_bool;
} pair_t;

// table-driven string comparison
static const pair_t bool_types_tbl[] = 
{
    // true conditions
    { "true",  true  },
    { "yes",   true  },
    { "on",    true  },
    { "1",     true  },
    // false conditions
    { "false", false },
    { "no",    false },
    { "off",   false },
    { "0",     false },
    // terminator
    { "",      false },
};

bool IniParser_GetBoolean (
    T P, const char *const section, const char *const name,
    const bool default_value )
{
    assert ( P );
    const char *val_str = IniParser_GetString ( P, section, name, "" );
    for ( int i = 0; *bool_types_tbl[i].val_str != '\0'; i++ )
    {
        if ( STREQ ( val_str, bool_types_tbl[i].val_str ) )
            return bool_types_tbl[i].val_bool;
    }
    return default_value;
}
