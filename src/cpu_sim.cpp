#include "cpu_sim.h"
#include "execution_history.h"

#include <utility>

CPUSim::CPUSim(std::unique_ptr<Scheduler> scheduler,
               const std::vector<proc_input_t> &procs)
    : scheduler(std::move(scheduler)) {
  for (const auto &[realname, burst, arrival] : procs) {
    proc_t proc = {.pid = new_pid(), .realname = realname, .burst = burst, .arrival = arrival};
    this->procs.insert({proc.pid, proc});
    this->arrivals.insert(proc.pid, proc.arrival);
    this->history.add_proc(proc.pid, proc.realname);
  }
}

int CPUSim::new_pid() {
  next_pid += 1;
  return next_pid;
}

void CPUSim::step() {
  history.step();
  if (const int curr_pid = arrivals.next(); curr_pid != -1) {
    scheduler->new_proc(curr_pid, procs.at(curr_pid).burst);
    history.add_event_arrived(curr_pid);
  }
  auto [pid, finished] = scheduler->step();
  if (finished) {
    pdone(pid);
    history.add_event_finished(pid);
  } else {
    history.add_event_running(pid);
  }
}

void CPUSim::pdone(const int pid) { procs.erase(pid); }

bool CPUSim::is_done() const {
  return procs.empty();
}

std::string CPUSim::format_history() const { return history.format_history(); }

void CPUSim::run() {
  while(!is_done()) {
    step();
  }
}
