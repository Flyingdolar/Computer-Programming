#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "linuxlist.h"
#include "schedule.h"

int main(void) {
    LIST_HEAD(jobList);
    LIST_HEAD(schedJobList);
    struct list_head *pNode;
    sJob *pJob;
    sSchedJob *pSchedJob;
    int ret;

    for (int idx = 0; idx < 5; idx++) {
        pJob = (sJob *)malloc(sizeof(sJob));
        pJob->id = idx;
        pJob->arrival = rand() % 10;
        pJob->duration = rand() % 10;
        add_job(&jobList, pJob);
    }

    ret = schedule(&jobList, &schedJobList);
    if (ret != 0) {
        printf("schedule() returned %d\n", ret);
        return -1;
    }

    // print job list
    printf("job list:\n");
    list_for_each(pNode, &jobList) {
        pJob = list_entry(pNode, sJob, list);
        printf("id: %u, arrival: %u, duration: %u\n", pJob->id, pJob->arrival, pJob->duration);
    }

    // print scheduled job list
    printf("scheduled job list:\n");
    list_for_each(pNode, &schedJobList) {
        pSchedJob = list_entry(pNode, sSchedJob, list);
        printf("id: %u, start: %u, stop: %u\n", pSchedJob->id, pSchedJob->start, pSchedJob->stop);
    }
    return 0;
}
