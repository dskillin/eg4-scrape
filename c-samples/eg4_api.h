#ifndef EG4_API_H
#define EG4_API_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

// Config structure
typedef struct {
    char account[64];
    char password[64];
    char serialNum[64];
    char plantId[16];
} eg4_config_t;

// Response structure for curl
typedef struct {
    char *data;
    size_t size;
} response_t;

// Function prototypes
int eg4_login(CURL *curl, const eg4_config_t *config);
void eg4_get_runtime(CURL *curl, const eg4_config_t *config);
void eg4_get_energy_info(CURL *curl, const eg4_config_t *config);
void eg4_get_month_column(CURL *curl, const eg4_config_t *config, int year, int month);
void eg4_get_day_multiline(CURL *curl, const eg4_config_t *config, const char *date);
void eg4_get_inverter_list(CURL *curl, const eg4_config_t *config);

#endif

