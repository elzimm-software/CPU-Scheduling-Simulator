#ifndef FIFO_H_
#define FIFO_H_

#include "scheduler.h"

#include <queue>

class FiFo : public Scheduler {
  std::queue<proc_burst_t> q;
  proc_burst_t running = {.proc = -1, .burst = 0};
public:
  running_proc_t step() override;
  void new_proc(int pid, int burst) override;
};

#endif // FIFO_H_
