#ifndef EXECUTION_HISTORY_H_
#define EXECUTION_HISTORY_H_

#include <map>
#include <string>
#include <vector>

struct HistoryTick {
  int arrived = -1; // pid that arrived this tick, or -1
  int running = -1; // pid that ran this tick, or -1
  bool finished = false; // whether `running` completed this tick
};

class ExecutionHistory {
  std::vector<HistoryTick> history;
  std::map<int, std::string> names;

  static constexpr size_t THROUGHPUT_CYCLES = 10;

  HistoryTick &current_tick();


public:
  void add_proc(int pid, const std::string &name);
  void add_event_arrived(int pid);
  void add_event_running(int pid);
  void add_event_finished(int pid);
  void step();
  [[nodiscard]] double average_waiting_time() const;
  [[nodiscard]] double average_response_time() const;
  [[nodiscard]] double throughput(size_t first_cycle, size_t last_cycle) const;
  [[nodiscard]] std::string format_history() const;
};

#endif // EXECUTION_HISTORY_H_
