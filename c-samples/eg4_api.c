#include "eg4_api.h"

#define BASE_URL "https://monitor.eg4electronics.com/WManage"
#define CONFIG_FILE "config.txt"
#define COOKIE_FILE "cookies.txt"

// Callback function for curl
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    response_t *resp = (response_t *)userp;

    char *ptr = realloc(resp->data, resp->size + realsize + 1);
    if(!ptr) {
        printf("Out of memory!\n");
        return 0;
    }

    resp->data = ptr;
    memcpy(&(resp->data[resp->size]), contents, realsize);
    resp->size += realsize;
    resp->data[resp->size] = 0;

    return realsize;
}

// Load configuration from file
static int load_config(eg4_config_t *config) {
    FILE *fp = fopen(CONFIG_FILE, "r");
    if (!fp) {
        printf("Error: Cannot open config file\n");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");
        if (key && value) {
            // Remove whitespace
            while (*value == ' ') value++;
            
            if (strcmp(key, "account") == 0)
                strncpy(config->account, value, sizeof(config->account)-1);
            else if (strcmp(key, "password") == 0)
                strncpy(config->password, value, sizeof(config->password)-1);
            else if (strcmp(key, "serialNum") == 0)
                strncpy(config->serialNum, value, sizeof(config->serialNum)-1);
            else if (strcmp(key, "plantId") == 0)
                strncpy(config->plantId, value, sizeof(config->plantId)-1);
        }
    }
    fclose(fp);
    return 1;
}

// Initialize curl with common options
static CURL* init_curl(void) {
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, COOKIE_FILE);
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, COOKIE_FILE);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    }
    return curl;
}

// Generic API call function
static char* make_api_call(CURL *curl, const char *url, const char *post_fields) {
    response_t resp = {0};
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        printf("API call failed: %s\n", curl_easy_strerror(res));
        free(resp.data);
        return NULL;
    }

    return resp.data;
}

// Perform login
int eg4_login(CURL *curl, const eg4_config_t *config) {
    char url[256];
    char post_fields[256];

    snprintf(url, sizeof(url), "%s/web/login", BASE_URL);
    snprintf(post_fields, sizeof(post_fields), 
             "account=%s&password=%s", 
             config->account, config->password);

    char *response = make_api_call(curl, url, post_fields);
    if (!response) return 0;

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    free(response);
    
    return (http_code == 200 || http_code == 302);
}

// Get runtime data
void eg4_get_runtime(CURL *curl, const eg4_config_t *config) {
    char url[256];
    char post_fields[256];

    snprintf(url, sizeof(url), "%s/api/inverter/getInverterRuntime", BASE_URL);
    snprintf(post_fields, sizeof(post_fields), "serialNum=%s", config->serialNum);

    char *response = make_api_call(curl, url, post_fields);
    if (response) {
        printf("%s\n", response);
        free(response);
    }
}

// Get energy info
void eg4_get_energy_info(CURL *curl, const eg4_config_t *config) {
    char url[256];
    char post_fields[256];

    snprintf(url, sizeof(url), "%s/api/inverter/getInverterEnergyInfo", BASE_URL);
    snprintf(post_fields, sizeof(post_fields), "serialNum=%s", config->serialNum);

    char *response = make_api_call(curl, url, post_fields);
    if (response) {
        printf("%s\n", response);
        free(response);
    }
}

// Get month column data
void eg4_get_month_column(CURL *curl, const eg4_config_t *config, int year, int month) {
    char url[256];
    char post_fields[512];

    snprintf(url, sizeof(url), "%s/api/inverterChart/monthColumn", BASE_URL);
    snprintf(post_fields, sizeof(post_fields), 
             "serialNum=%s&year=%d&month=%d",
             config->serialNum, year, month);

    char *response = make_api_call(curl, url, post_fields);
    if (response) {
        printf("%s\n", response);
        free(response);
    }
}

// Get day multiline data
void eg4_get_day_multiline(CURL *curl, const eg4_config_t *config, const char *date) {
    char url[256];
    char post_fields[512];

    snprintf(url, sizeof(url), "%s/api/analyze/chart/dayMultiLine", BASE_URL);
    snprintf(post_fields, sizeof(post_fields), 
             "serialNum=%s&dateText=%s",
             config->serialNum, date);

    char *response = make_api_call(curl, url, post_fields);
    if (response) {
        printf("%s\n", response);
        free(response);
    }
}

// Get inverter list
void eg4_get_inverter_list(CURL *curl, const eg4_config_t *config) {
    char url[256];
    char post_fields[512];

    snprintf(url, sizeof(url), "%s/web/config/inverter/list", BASE_URL);
    snprintf(post_fields, sizeof(post_fields), 
             "page=1&rows=20&plantId=%s&searchText=&targetSerialNum=%s",
             config->plantId, config->serialNum);

    char *response = make_api_call(curl, url, post_fields);
    if (response) {
        printf("%s\n", response);
        free(response);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("  %s runtime\n", argv[0]);
        printf("  %s energy\n", argv[0]);
        printf("  %s month <month> <year>\n", argv[0]);
        printf("  %s day <day> <month> <year>\n", argv[0]);
        printf("  %s list\n", argv[0]);
        return 1;
    }

    eg4_config_t config;
    if (!load_config(&config)) {
        return 1;
    }

    CURL *curl = init_curl();
    if (!curl) {
        printf("Error: Could not initialize CURL\n");
        return 1;
    }

    // Perform login first
    if (!eg4_login(curl, &config)) {
        printf("Login failed\n");
        curl_easy_cleanup(curl);
        return 1;
    }

    printf("Login successful\n");

    // Process command
    if (strcmp(argv[1], "runtime") == 0) {
        eg4_get_runtime(curl, &config);
    }
    else if (strcmp(argv[1], "energy") == 0) {
        eg4_get_energy_info(curl, &config);
    }
    else if (strcmp(argv[1], "month") == 0) {
        if (argc != 4) {
            printf("Usage: %s month <month> <year>\n", argv[0]);
            return 1;
        }
        eg4_get_month_column(curl, &config, atoi(argv[3]), atoi(argv[2]));
    }
    else if (strcmp(argv[1], "day") == 0) {
        if (argc != 5) {
            printf("Usage: %s day <day> <month> <year>\n", argv[0]);
            return 1;
        }
        char date[11];  // YYYY-MM-DD + null terminator
        snprintf(date, sizeof(date), "%s-%02d-%02d", 
                argv[4],           // year
                atoi(argv[3]),     // month
                atoi(argv[2]));    // day
        eg4_get_day_multiline(curl, &config, date);
    }
    else if (strcmp(argv[1], "list") == 0) {
        eg4_get_inverter_list(curl, &config);
    }
    else {
        printf("Unknown command: %s\n", argv[1]);
    }

    curl_easy_cleanup(curl);
    return 0;
}

