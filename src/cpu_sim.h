#ifndef CPU_SIM_H_
#define CPU_SIM_H_

#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_map>

#include "arrival_list.h"
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
  ArrivalList arrivals;
  int next_pid = 0;

  int new_pid();

public:
  explicit CPUSim(std::unique_ptr<Scheduler> scheduler,
                  std::initializer_list<proc_input_t> procs = {});
  void step();
  void pdone(int pid);
};

#endif // CPU_SIM_H_
