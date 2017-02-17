#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/ini_parser.h"

int main()
{
    IniParser_T p = IniParser_Create("test.ini");
    printf("Is errort: %d\n", IniParser_CheckError(p));

    const char * v1 = IniParser_Get(p, "user", "name", "taka");
    printf("user-name: %s\n", v1);

    long v2 = IniParser_GetInteger(p, "protocol", "version", 4);
    printf("protocol-version: %ld\n", v2);

    double v3 = IniParser_GetDouble(p, "user", "pi", 10.2);
    printf("user-pi: %f\n", v3);

    bool v4 = IniParser_GetBoolean(p, "user", "active", false);
    printf("user-active: %d\n", v4);

    IniParser_Destroy(p);
    
    return 0;
}