#ifndef EXECUTION_HISTORY_H_
#define EXECUTION_HISTORY_H_

#include <map>
#include <string>
#include <vector>

enum EventType {
  ARRIVED,
  FINISHED,
  RUNNING,
};

class HistoryEvent {
  EventType event;
  int pid;
public:
  HistoryEvent(int pid, EventType event);
  [[nodiscard]] int get_pid() const;
  [[nodiscard]] EventType get_event() const;
};

class ExecutionHistory {
  std::vector<std::vector<HistoryEvent>> history;
  std::map<int, std::string> names;

  static constexpr size_t THROUGHPUT_CYCLES = 10;

  [[nodiscard]] bool has_event(size_t tick, int pid, EventType type) const;
  [[nodiscard]] bool is_running(size_t tick, int pid) const;

public:
  void add_proc(int pid, const std::string &name);
  void add_event(int pid, EventType event);
  void step();
  [[nodiscard]] double average_waiting_time() const;
  [[nodiscard]] double average_response_time() const;
  [[nodiscard]] double throughput(size_t first_cycle, size_t last_cycle) const;
  [[nodiscard]] std::string format_history() const;
};

#endif // EXECUTION_HISTORY_H_
