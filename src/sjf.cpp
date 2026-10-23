#include "sjf.h"


Scheduler::running_proc_t SJF::step() {
    if (q.empty()) {
        return {.pid = -1, .finished = false};
    }
    proc_burst_t running = q.top();
    q.pop();
    running.burst -= 1;
    bool finished = false;
    if (running.burst == 0) {
        finished = true;
    } else {
        q.push(running);
    }
    return {.pid = running.proc, .finished = finished};
}

void SJF::new_proc(const int pid, const int burst) {
    q.push({.proc = pid, .burst = burst});
}
