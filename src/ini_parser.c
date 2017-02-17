#include "ini_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "internal/ini.h"
#include "internal/uthash.h"

#define MAX_KEY_LEN   100   // max key length for hashtable
#define MAX_VALUE_LEN 255   // max value length for hashtable
#define T IniParser_T       // IniParser_S pointer (Parser class)
#define KV KV_T             // KV_S pointer (key value structure for hashtable)

// INI parser structure
struct IniParser_S 
{
    struct KV_S *hashtable;
    int error;
};

typedef struct KV_S* KV_T;

// internal key value item structure for hashtable
struct KV_S
{
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    UT_hash_handle hh;  // makes this structure hashable
};


// ======================================
// private methods section
// ======================================

// make_key concat section and name string as hash key
static void make_key(const char* const section, 
                     const char* const name, 
                     char* key_arr, 
                     size_t key_arr_len)
{
    snprintf(key_arr, key_arr_len, "%s@@%s", section, name);
}

// hash_add insert key value to hash table
static void hash_add(T P, const char* const key, 
                          const char* const value)
{
    KV s = NULL;
    s = malloc(sizeof(struct KV_S));
    strncpy(s->key, key, strlen(key));
    strncpy(s->value, value, strlen(value));
    HASH_ADD_STR(P->hashtable, key, s );
}

// hash_get get value from hashtable via key,
//  return char pointer is maintained by hashtable
static char* hash_get(T P, const char* const key)
{
    KV s = NULL;
    HASH_FIND_STR(P->hashtable, key, s);
    return ( NULL != s ) ? s->value: "";
}

// ini parser handler
static int handler( void* user, 
                    const char* section, 
                    const char* name, 
                    const char* value)
{
    T P = (T)user;
    char key[MAX_KEY_LEN];
    make_key(section, name, key, MAX_KEY_LEN);
    hash_add(P, key, value);
    return 1;
}

// ======================================
// public methods section
// ======================================

T IniParser_Create(const char* const filename)
{
    T P = NULL;
    P = (T)malloc(sizeof(struct IniParser_S));
    if (NULL == P)
    {
        return NULL;
    }
    
    P->hashtable = NULL;                        // init hash table
    P->error = ini_parse(filename, handler, P); // load ini
    return P;
}

void IniParser_Destroy(T P)
{
     assert(P);
     HASH_CLEAR(hh, P->hashtable); // clear hashtable
     
     free(P->hashtable);
     P->hashtable = NULL;
     
     free(P);
     P = NULL;
}

int IniParser_CheckError(T P)
{
    assert(P);
    return P->error;
}

const char* const IniParser_Get(T P, const char* const section, 
                                     const char* const name, 
                                     const char* const default_value)
{
    assert(P);
    char key[MAX_KEY_LEN];
    make_key(section, name, key, MAX_KEY_LEN);
    char* v = hash_get(P, key);
    return strlen(v) > 0 ? v : default_value;
}

long IniParser_GetInteger(T P, const char* const section, 
                               const char* const name, 
                               const long default_value)
{
    assert(P);
    const char* val_str = IniParser_Get(P, section, name, "");
    char* end;
    long val_l = strtol(val_str, &end, 0);
    return end > val_str ? val_l : default_value;

}

double IniParser_GetDouble(T P, const char* const section, 
                                const char* const name, 
                                const double default_value)
{
    assert(P);
    const char* val_str = IniParser_Get(P, section, name, "");
    char* end;
    double val_d = strtod(val_str, &end);
    return end > val_str ? val_d : default_value;
}

bool IniParser_GetBoolean(T P, const char* const section, 
                               const char* const name, 
                               const bool default_value)
{
    assert(P);
    const char* val_str = IniParser_Get(P, section, name, "");

    if (!strcasecmp(val_str, "true") || 
        !strcasecmp(val_str, "yes")  || 
        !strcasecmp(val_str, "on")   || 
        !strcasecmp(val_str, "1")     )
    {
        return true;
    } 
    else if (!strcasecmp(val_str, "false") || 
             !strcasecmp(val_str, "no")    || 
             !strcasecmp(val_str, "off")   || 
             !strcasecmp(val_str, "0")      )
    {
        return false;
    }
    else
    {
        return default_value;
    }
}
