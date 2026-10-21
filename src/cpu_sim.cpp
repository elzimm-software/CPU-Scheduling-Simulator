#include "cpu_sim.h"

#include <utility>

CPUSim::CPUSim(std::unique_ptr<Scheduler> scheduler,
               std::initializer_list<proc_input_t> procs)
    : scheduler(std::move(scheduler)) {
  for (const auto &input : procs) {
    proc_t proc = {new_pid(), input.realname, input.burst, input.arrival};
    this->procs.insert({proc.pid, proc});
    this->arrivals.insert(proc.pid, proc.arrival);
  }
}

int CPUSim::new_pid() {
  next_pid += 1;
  return next_pid;
}

void CPUSim::step() {
  Scheduler::running_proc_t running = scheduler->step();
  // add running pid to history
  if (running.finished) {
    pdone(running.pid);
  }
}

void CPUSim::pdone(int pid) { procs.erase(pid); }
