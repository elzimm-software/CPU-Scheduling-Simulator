#ifndef CPU_SIM_H_
#define CPU_SIM_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "arrival_list.h"
#include "execution_history.h"
#include "scheduler.h"

class CPUSim {
public:
  struct proc_input_t {
    std::string realname;
    int burst;
    int arrival;
  };

private:
  struct proc_t {
    int pid;
    std::string realname;
    int burst;
    int arrival;
  };

  std::unique_ptr<Scheduler> scheduler;
  std::unordered_map<int, proc_t> procs;
  ExecutionHistory history;
  ArrivalList arrivals;
  int next_pid = 0;

  int new_pid();
  void pdone(int pid);

public:
  explicit CPUSim(std::unique_ptr<Scheduler> scheduler,
                  const std::vector<proc_input_t> &procs = {});
  void step();
  bool is_done() const;
  void run();
  std::string format_history() const;
};

#endif // CPU_SIM_H_
