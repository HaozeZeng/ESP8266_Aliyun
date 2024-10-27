#ifndef DATAPARSE_H
#define DATAPARSE_H
#include "mqttbasedef.h"

struct receiveData *parse_json(const char *json_data, uint16_t len);
struct receiveData *parse_json_2(const char *json_data, uint16_t len);

struct receiveData
{
    bool receiveed;
    char *PosterName;
    char *NameLen;
    uint16_t CurrentTemperature;
    bool LightSwitch;
};
#endif // JSON_H