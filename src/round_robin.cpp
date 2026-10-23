#include "round_robin.h"

Scheduler::running_proc_t RoundRobin::step() {
    const bool proc_ok = running.proc != -1 && running.burst > 0;
    if (q.empty() && !proc_ok) {
        return {.pid = -1, .finished = false};
    }
    if (proc_ok) {
        q.push(running);
    }
    running = q.front();
    q.pop();
    running.burst -= 1;
    bool finished = false;
    if (running.burst <= 0) {
        finished = true;
    }
    return {.pid = running.proc, .finished = finished};
}

void RoundRobin::new_proc(const int pid, const int burst) {
    q.push({.proc = pid, .burst = burst});
}
