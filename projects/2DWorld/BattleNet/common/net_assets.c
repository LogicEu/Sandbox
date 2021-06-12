#include "BNCommon.h"

void read_net_file(const char* path, char* host, uint16_t* port)
{
    char str[128];
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Could not open file '%s'.\n", path);
        exit(-1);
    } else fscanf(file, "%s", &str[0]);
    fclose(file);

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ':') {
            host[i] = '\0';
            *port = (uint16_t)atoi(&str[i + 1]);
            break;
        }
        else host[i] = str[i];
    }
}