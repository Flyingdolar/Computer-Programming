#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PRINTE(...) printf("\033[0;31mE: "), printf(__VA_ARGS__), printf("\033[0m\n")

struct option long_options[] = {
    {"interval", 1, NULL, 'i'},
    {"count", 1, NULL, 'c'},
    {"timestamp", 0, NULL, 't'},
    {"help", 0, NULL, 'h'},
    {NULL, 0, NULL, 0}};

typedef enum _optCmd_ {
    INTERVAL,
    COUNT,
    TIMESTAMP,
    HELP
} optCmd;

void printHelp() {
    printf("\033[0;33mUsing Help Command...\033[0m\n");
    printf("+==============================+\n");
    printf("| HW0401 - Process Information |\n");
    printf("+==============================+\n");
    printf("It is a command to show your computer current status\n");

    printf("\n* Usages *\n");
    printf("hw0401\t[Options]...\t[PID]\n");
    printf("\n* Options *\n");
    printf(" %s / %-10s\t | %-15s\t | %-20s\n", "Short", "Long Opt.", "Variable (Default)", "Function");
    printf("-------------------------+-----------------------+---------------------------------\n");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-i", "--interval", "[int]:1~100(1)", "Display the process information every [int]");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-c", "--count", "[int]:1~10000(INF.)", "Display the process information [int] times.");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-t", "--timestamp", "", "Display the timestamp.");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-h", "--help", "", "print this help");
    return;
}

bool isInvalid(bool cmd[], int interval, int count, char *pidStr) {
    if (cmd[INTERVAL] && (interval < 1 || interval > 100)) {
        PRINTE("Invalid interval value: %d", interval);
        return true;
    }
    if (cmd[COUNT] && (count < 1 || count > 10000)) {
        PRINTE("Invalid count value: %d", count);
        return true;
    }
    if (pidStr == NULL) {
        PRINTE("No PID input");
        return true;
    }
    if (pidStr[0] != '0' && atoi(pidStr) == 0) {
        PRINTE("Invalid PID: %s", pidStr);
        return true;
    }
    return false;
}

char **getStat(char *pidStr) {
    char **stat = (char **)malloc(sizeof(char *) * 52);
    char *path = (char *)malloc(sizeof(char) * 100);
    sprintf(path, "/proc/%s/stat", pidStr);
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        PRINTE("No such process: %s", pidStr);
        return NULL;
    }
    char *buf = (char *)malloc(sizeof(char) * 1000);
    fgets(buf, 1000, fp);
    char *token = strtok(buf, " ");
    for (int idx = 0; token != NULL; idx++) {
        stat[idx] = strdup(token);
        token = strtok(NULL, " ");
    }
    fclose(fp);
    return stat;
}

void freeStat(char **stat) {
    for (int idx = 0; idx < 52; idx++) free(stat[idx]);
    free(stat);
    return;
}

int getTotalMemory() {
    FILE *fp = fopen("/proc/meminfo", "r");
    char *buf = (char *)malloc(sizeof(char) * 1000);
    fgets(buf, 1000, fp);
    char *token = strtok(buf, " ");
    token = strtok(NULL, " ");
    fclose(fp);
    return atoi(token) * 1024;
}

int main(const int argc, char *const argv[]) {
    bool cmd[4] = {false};
    int opt, interval = 1, count = 0;
    double cpuUsage, memUsage, lastTime;
    char sysCmd[1000] = "sed -e 's/ /\n/g' /proc/";
    char *pidStr, **pidStat, *pidCmd;

    while ((opt = getopt_long(argc, argv, "i:c:th", long_options, NULL)) != -1) {
        if (opt == 'i')
            cmd[INTERVAL] = true, interval = atoi(optarg);
        if (opt == 'c')
            cmd[COUNT] = true, count = atoi(optarg);
        if (opt == 't')
            cmd[TIMESTAMP] = true;
        if (opt == 'h')
            cmd[HELP] = true, printHelp();
        if (opt == '?') {
            printf("Unknown option: %c\n", optopt);
            return -1;
        }
    }
    if (optind < argc) pidStr = strdup(argv[optind]);

    if (cmd[HELP]) return 0;
    if (isInvalid(cmd, interval, count, pidStr)) return -1;

    // Get lastTime
    pidStat = getStat(pidStr);
    lastTime = atof(pidStat[13]) + atof(pidStat[14]);
    freeStat(pidStat), sleep(interval);

    while (!cmd[COUNT] || count--) {
        pidStat = getStat(pidStr);
        pidCmd = strdup(pidStat[1]);
        cpuUsage = (atof(pidStat[13]) + atof(pidStat[14]) - lastTime) * 100 / (interval * sysconf(_SC_CLK_TCK));
        memUsage = atof(pidStat[23]) * sysconf(_SC_PAGESIZE) * 100 / getTotalMemory();
        lastTime = atof(pidStat[13]) + atof(pidStat[14]);
        freeStat(pidStat);

        if (cmd[TIMESTAMP]) {
            FILE *fp = popen("date +\"%Y-%m-%d %H:%M:%S\"", "r");
            fgets(sysCmd, 1000, fp), strtok(sysCmd, "\n");
            printf("%s", sysCmd);
            pclose(fp);
        }

        printf(" %.1lf%%[CPU]", cpuUsage);
        printf(" %.1lf%%[MEM]", memUsage);
        printf(" %s[COMMAND]\n", strtok(pidCmd, "()"));
        free(pidCmd), sleep(interval);
    }
    free(pidStr);
    return 0;
}
