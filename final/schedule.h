#pragma once

#include <stdint.h>

#include "linuxlist.h"

typedef struct _sJob {
    uint32_t id;
    uint32_t arrival;
    uint32_t duration;

    struct list_head list;
} sJob;

typedef struct _sSchedJob {
    uint32_t id;
    uint32_t start;
    uint32_t stop;

    struct list_head list;
} sSchedJob;

// Add pJob to pJobList.
// Return -1 if there is any NULL pointer; otherwise , return 0.
int add_job(struct list_head *pJobList, sJob *pJob);

// According to pJobList , schedule all jobs and stored in pSchedJobList.
// All data stored in pSchedJobList is sSchedJob.
// Note that pSchedJobList should be sorted in the start time ascending order.
// Return -1 if there is any NULL pointer; otherwise , return 0.
// Each entry in pJobList is sJob while each entry in pSchedJobList is sSchedJob
int schedule(struct list_head *pJobList, struct list_head *pSchedJobList);
