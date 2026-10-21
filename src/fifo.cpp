#include "fifo.h"
#include "cpu_sim.h"

Scheduler::running_proc_t FiFo::step() {
  if (running.burst <= 0) {
    if (q.empty()) {
      running = {.proc = -1, .burst = 0};
      return {.pid = -1, .finished = false};
    }
    running = q.front();
    q.pop();
  }
  running.burst -= 1;
  bool finished = false;
  if (running.burst == 0) {
    finished = true;
  }
  return {.pid = running.proc, .finished = finished};
}

void FiFo::new_proc(const int pid, const int burst) {
  this->q.push({.proc = pid, .burst = burst});
}
