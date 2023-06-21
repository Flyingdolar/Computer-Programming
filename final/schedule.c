#include "schedule.h"

#include "linuxlist.h"

int add_job(struct list_head *pJobList, sJob *pJob) {
    if (pJobList == NULL) return -1;
    if (pJob == NULL) return -1;
    if (pJob->arrival < 0 || pJob->duration < 0) return -1;
    list_add_tail(&pJob->list, pJobList);
    return 0;
}

int schedule(struct list_head *pJobList, struct list_head *pSchedJobList) {
    int len = 0, idx = 0;
    struct list_head *curNode;
    LIST_HEAD(tmpList);
    sJob *curJob, *tmpJob, *nextJob;

    if (pJobList == NULL) return -1;
    if (pSchedJobList == NULL) return -1;

    list_for_each(curNode, pJobList) {
        curJob = list_entry(curNode, sJob, list);
        tmpJob = (sJob *)malloc(sizeof(sJob));
        tmpJob->arrival = curJob->arrival, tmpJob->duration = curJob->duration;
        tmpJob->id = curJob->id;
        list_add_tail(&tmpJob->list, &tmpList);
    }

    int time = 0;
    while (tmpList.next != &tmpList) {
        nextJob = NULL;
        list_for_each(curNode, &tmpList) {
            curJob = list_entry(curNode, sJob, list);
            if (nextJob == NULL) {
                nextJob = curJob;
                continue;
            }
            if (curJob->arrival <= time) {
                if (nextJob->arrival > time)
                    nextJob = curJob;
                else if (nextJob->duration > curJob->duration)
                    nextJob = curJob;
                continue;
            }
            if (nextJob->arrival > curJob->arrival) nextJob = curJob;
            if (nextJob->arrival == curJob->arrival && nextJob->duration > curJob->duration) nextJob = curJob;
        }
        if (nextJob->arrival > time) time = nextJob->arrival;
        sSchedJob *newJob = (sSchedJob *)malloc(sizeof(sSchedJob));
        newJob->id = nextJob->id, newJob->start = time, newJob->stop = time + nextJob->duration;
        newJob->list.next = newJob->list.prev = NULL;
        list_add_tail(&(newJob->list), pSchedJobList);
        list_del(&nextJob->list), free(nextJob);
        time = newJob->stop;
    }
    return 0;
}
