#ifndef SJF_H_
#define SJF_H_

#include <queue>

#include "scheduler.h"


class SJF: public Scheduler {

    struct ShortestJob {
        bool operator()(const proc_burst_t& a, const proc_burst_t& b) const {
            return a.burst > b.burst;
        }
    };

    std::priority_queue<proc_burst_t, std::vector<proc_burst_t>, ShortestJob> q;

public:
    running_proc_t step() override;
    void new_proc(int pid, int burst) override;
};

#endif // SJF_H_