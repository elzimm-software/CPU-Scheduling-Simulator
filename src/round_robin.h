#ifndef ROUND_ROBIN_H_
#define ROUND_ROBIN_H_

#include <queue>
#include "scheduler.h"

/**
 * Round Robin Scheduling Algorithm
 *
 * There exists a ready queue.
 * As they arrive, processes are added to this queue.
 * At each execution step, the top process is removed from the queue and executed.
 * After its execution, it is inserted back into end of the queue.
 * Processes that arrive before the next step are inserted prior to the running proc.
 */
class RoundRobin: public Scheduler {
    std::queue<proc_burst_t> q;
    proc_burst_t running = {.proc = -1, .burst = 0};
public:
    running_proc_t step() override;
    void new_proc(int pid, int burst) override;
};


#endif //ROUND_ROBIN_H_
