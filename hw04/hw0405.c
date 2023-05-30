#include <curl/curl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "cJSON.h"

#define PRINTE(...) printf("\033[0;31mE: "), printf(__VA_ARGS__), printf("\033[0m\n")

typedef enum _optCmd_ {
    SYMBOL,
    INPUT,
    OUTPUT,
    HELP
} optCmd;

void printHelp() {
    printf("\033[0;33mUsing Help Command...\033[0m\n");
    printf("+==============================+\n");
    printf("| HW0405 - Stock Info Collector |\n");
    printf("+==============================+\n");
    printf("Collect stock data from the yahoo finance\n");

    printf("\n* Usages *\n");
    printf("hw0405\t-i \n");
    printf("\n* Options *\n");
    printf(" %s / %-10s\t | %-15s\t | %-20s\n", "Short", "Long Opt.", "Variable (Default)", "Function");
    printf("-------------------------+-----------------------+---------------------------------\n");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-s", "--symbol", "[STOCK ID]", "Symbol of stock");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-i", "--input", "[FILE NAME]", "Input JSON file name");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-o", "--output", "[FILE NAME]", "Output JSON file name");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-h", "--help", "", "print this help");
    return;
}

struct option long_options[] = {
    {"symbol", 1, NULL, 's'},
    {"input", 1, NULL, 'i'},
    {"output", 1, NULL, 'o'},
    {"help", 0, NULL, 'h'},
    {NULL, 0, NULL, 0}};

void getData(char *file) {
    char grep[1024] = "grep -o '<span>[^<]*</span>'";
    char sed[1024] = "sed 's/<span>//g; s/<\\/span>//g; s/<\\/\?span>/ /g'";
    char sed2[1024] = "sed -i '1,/Volume/d' ";
    // Remove all the line after "*close"
    char sed3[1024] = "sed -i '/\\*Close/,$d'";
    char cmd[4096], tmp[4096], cmd2[4096], cmd3[4096];

    snprintf(tmp, sizeof(tmp), "mv %s tmp1.txt", file), system(tmp);
    snprintf(cmd, sizeof(cmd) - 1, "%s tmp1.txt | %s > %s", grep, sed, file), system(cmd);
    system("rm tmp1.txt");
    snprintf(cmd2, sizeof(cmd2) - 1, "%s %s", sed2, file), system(cmd2);
    snprintf(cmd3, sizeof(cmd3) - 1, "%s %s", sed3, file), system(cmd3);
    return;
}

int getStockInfo(char *stock, time_t stDate, time_t edDate, char *file) {
    char web[1024] = "https://finance.yahoo.com/quote/";
    char tmp[1024], cmd[1024];
    char start[32], end[32];
    CURL *curl;
    FILE *fp = fopen(file, "w");

    if (fp == NULL) {
        PRINTE("Cannot create temproray file: %s", file);
        return -1;
    }
    snprintf(start, sizeof(start), "%ld", stDate);
    snprintf(end, sizeof(end), "%ld", edDate);
    strcat(web, stock);
    strcat(web, "/history?period1="), strcat(web, start);
    strcat(web, "&period2="), strcat(web, end);
    strcat(web, "&interval=1d&filter=history&frequency=1d");

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, web);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        if (curl_easy_perform(curl) != CURLE_OK) {
            PRINTE("Cannot get URL: %s", web);
            return -1;
        }
        curl_easy_cleanup(curl);
    }
    fclose(fp);
    getData(file);
    return 0;
}

char *readFile(char *file) {
    FILE *fp = fopen(file, "r");
    char *str;
    long fileLen;

    if (fp == NULL) {
        PRINTE("Cannot open file: %s", file);
        return NULL;
    }
    fseek(fp, 0, SEEK_END), fileLen = ftell(fp) + 1, fseek(fp, 0, SEEK_SET);
    str = malloc(fileLen * sizeof(char));
    fread(str, 1, fileLen - 1, fp), str[fileLen - 1] = '\0';
    fclose(fp);
    return str;
}

time_t strToTime(char *str) {
    struct tm tm;
    tm.tm_year = atoi(str) - 1900;
    tm.tm_mon = atoi(str + 5) - 1;
    tm.tm_mday = atoi(str + 8);
    tm.tm_hour = 0, tm.tm_min = 0, tm.tm_sec = 0;
    return mktime(&tm);
}

int writeStockInfo(char *log, cJSON *objList) {
    FILE *fLog = fopen(log, "r");
    cJSON *obj = cJSON_CreateObject(), *dataList = cJSON_CreateArray(), *data;
    char date[100], tmp[100];
    double open, high, low, close, adjClose, min = 0, max = 0;
    int volume, count = 0;

    if (fLog == NULL) {
        if (fLog == NULL) PRINTE("Cannot open file: %s", log);
        return -1;
    }

    while (1) {
        fgets(date, sizeof(date), fLog);
        fgets(tmp, sizeof(tmp), fLog), open = atof(tmp);
        fgets(tmp, sizeof(tmp), fLog), high = atof(tmp);
        fgets(tmp, sizeof(tmp), fLog), low = atof(tmp);
        fgets(tmp, sizeof(tmp), fLog), close = atof(tmp);
        fgets(tmp, sizeof(tmp), fLog), adjClose = atof(tmp);
        fgets(tmp, sizeof(tmp), fLog), volume = atoi(tmp);
        if (feof(fLog)) break;
        if (min > low || min == 0) min = low;
        if (max < high) max = high;
        data = cJSON_CreateObject(), ++count;
        cJSON_AddStringToObject(data, "date", date);
        cJSON_AddNumberToObject(data, "open", open);
        cJSON_AddNumberToObject(data, "high", high);
        cJSON_AddNumberToObject(data, "low", low);
        cJSON_AddNumberToObject(data, "close", close);
        cJSON_AddNumberToObject(data, "volume", volume);
        cJSON_AddItemToArray(dataList, data);
    }

    if (count > 0) {
        cJSON_AddItemToObject(obj, "data", dataList);
        cJSON_AddNumberToObject(obj, "min", min);
        cJSON_AddNumberToObject(obj, "max", max);
    } else {
        cJSON_AddNullToObject(obj, "data");
        cJSON_AddNullToObject(obj, "min");
        cJSON_AddNullToObject(obj, "max");
    }
    cJSON_AddItemToArray(objList, obj);
    return 0;
}

int writeFile(char *file, cJSON *objList) {
    FILE *fp = fopen(file, "w");
    char *str = cJSON_Print(objList);

    if (fp == NULL) {
        PRINTE("Cannot open file: %s", file);
        return -1;
    }
    fprintf(fp, "%s", str);
    fclose(fp);
    return 0;
}

int isInvalid(bool *cmd, char *inFile, char *outFile, char *stock) {
    if (!cmd[SYMBOL]) {
        PRINTE("No symbol input");
        return -1;
    }
    if (!cmd[INPUT]) {
        PRINTE("No input file");
        return -1;
    }
    if (!cmd[OUTPUT]) {
        PRINTE("No output file");
        return -1;
    }
    if (strcmp(inFile, outFile) == 0) {
        PRINTE("Input file and output file are the same");
        return -1;
    }
    if (inFile == NULL) {
        PRINTE("No input file");
        return -1;
    }
    if (outFile == NULL) {
        PRINTE("No output file");
        return -1;
    }
    if (stock == NULL) {
        PRINTE("No stock symbol");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    char tmpFile[100] = "tmp.txt", inFile[1024], outFile[1024];
    char stock[100], *file, *stDate, *edDate;
    bool cmd[4] = {false};
    time_t stTime, edTime;
    cJSON *query, *result, *qElement;
    int opt;

    while ((opt = getopt_long(argc, argv, "s:i:o:h", long_options, NULL)) != -1) {
        if (opt == 's')
            cmd[SYMBOL] = true, strcpy(stock, optarg);
        if (opt == 'i')
            cmd[INPUT] = true, strcpy(inFile, optarg);
        if (opt == 'o')
            cmd[OUTPUT] = true, strcpy(outFile, optarg);
        if (opt == 'h')
            cmd[HELP] = true, printHelp();
        if (opt == '?') {
            printf("Unknown option: %c\n", optopt);
            return -1;
        }
    }
    if (cmd[HELP]) return 0;
    if (isInvalid(cmd, inFile, outFile, stock)) return -1;
    file = readFile(inFile);
    if (file == NULL) return -1;

    query = cJSON_Parse(file);
    result = cJSON_CreateArray();
    for (int idx = 0; idx < cJSON_GetArraySize(query); ++idx) {
        qElement = cJSON_GetArrayItem(query, idx);
        stDate = cJSON_GetObjectItem(qElement, "start")->valuestring;
        edDate = cJSON_GetObjectItem(qElement, "end")->valuestring;
        stTime = strToTime(stDate), edTime = strToTime(edDate);
        getStockInfo(stock, stTime, edTime, tmpFile);
        writeStockInfo(tmpFile, result);
        system("rm tmp.txt");
        qElement = qElement->next;
    }
    if (writeFile(outFile, result) != 0) return -1;
    return 0;
}
