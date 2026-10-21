#ifndef SCHEDULER_H_
#define SCHEDULER_H_

class Scheduler {
protected:
  struct proc_burst_t {
    int proc;
    int burst;
  };
public:
  struct running_proc_t {
    int pid;
    bool finished;
  };

  virtual ~Scheduler() = default;

  virtual void new_proc(int pid, int burst) = 0;

  /**
   * Performs one step of the CPU.
   * Returns the running proc and if it is finished.
   **/
  virtual running_proc_t step() = 0;
};

#endif // SCHEDULER_H_
