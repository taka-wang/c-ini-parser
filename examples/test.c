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
