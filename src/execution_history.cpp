#include "execution_history.h"

#include <algorithm>
#include <cstdio>
#include <stdexcept>

void ExecutionHistory::add_proc(const int pid, const std::string &name) {
  names[pid] = name;
}

HistoryTick &ExecutionHistory::current_tick() {
  if (history.empty()) {
    throw std::logic_error("add_event called before step()");
  }
  return history.back();
}

void ExecutionHistory::add_event_arrived(const int pid) {
  HistoryTick &tick = current_tick();
  if (tick.arrived != -1) {
    throw std::logic_error("duplicate arrived event in one tick");
  }
  tick.arrived = pid;
}

void ExecutionHistory::add_event_running(const int pid) {
  HistoryTick &tick = current_tick();
  if (tick.running != -1) {
    throw std::logic_error("duplicate running event in one tick");
  }
  tick.running = pid;
}

// The finishing tick is also a running tick for that process.
void ExecutionHistory::add_event_finished(const int pid) {
  HistoryTick &tick = current_tick();
  if (tick.finished) {
    throw std::logic_error("duplicate finished event in one tick");
  }
  if (tick.running != -1) {
    throw std::logic_error("duplicate running event in one tick");
  }
  tick.running = pid;
  tick.finished = true;
}

/**
 * Starts a new tick. Must be called before the first add_event_* of each tick.
 **/
void ExecutionHistory::step() {
  history.emplace_back();
}

/**
 * Mean over arrived processes of the ticks spent ready but not running, from
 * arrival until completion (or the end of the history if unfinished).
 **/
double ExecutionHistory::average_waiting_time() const {
  int total = 0;
  int count = 0;
  for (const auto &[pid, name] : names) {
    bool arrived = false;
    for (const auto &[tick_arrived, tick_running, tick_finished] : history) {
      if (tick_arrived == pid) {
        arrived = true;
      }
      if (!arrived) {
        continue;
      }
      if (tick_running != pid) {
        total++;
      }
      if (tick_running == pid && tick_finished) {
        break;
      }
    }
    if (arrived) {
      count++;
    }
  }
  return count ? static_cast<double>(total) / count : 0.0;
}

/**
 * Mean over processes that have run of the ticks between arrival and first
 * getting the CPU.
 **/
double ExecutionHistory::average_response_time() const {
  int total = 0;
  int count = 0;
  for (const auto &[pid, name] : names) {
    bool arrived = false;
    size_t arrival = 0;
    for (size_t t = 0; t < history.size(); t++) {
      if (!arrived && history[t].arrived == pid) {
        arrived = true;
        arrival = t;
      }
      if (arrived && history[t].running == pid) {
        total += static_cast<int>(t - arrival);
        count++;
        break;
      }
    }
  }
  return count ? static_cast<double>(total) / count : 0.0;
}

/**
 * Processes finished per cycle over ticks [first_cycle, last_cycle), clamped to
 * the ticks recorded so far.
 **/
double ExecutionHistory::throughput(const size_t first_cycle,
                                    size_t last_cycle) const {
  last_cycle = std::min(last_cycle, history.size());
  if (first_cycle >= last_cycle) {
    return 0.0;
  }
  int finished = 0;
  for (size_t t = first_cycle; t < last_cycle; t++) {
    if (history[t].finished) {
      finished++;
    }
  }
  return static_cast<double>(finished) / static_cast<double>(last_cycle - first_cycle);
}

/**
 * One line per process: its name, padding so that all histories line up two
 * spaces past the longest name, then one character per tick:
 * ' ' not in the system, '_' waiting, '#' running.
 **/
std::string ExecutionHistory::format_history() const {
  size_t width = 0;
  for (const auto &[pid, name] : names) {
    width = std::max(width, name.size());
  }

  std::string out;
  for (const auto &[pid, name] : names) {
    out += name + std::string(width - name.size() + 2, ' ');
    bool arrived = false;
    bool done = false;
    for (const auto &tick : history) {
      if (tick.arrived == pid) {
        arrived = true;
      }
      const bool running = tick.running == pid;
      const bool finished = running && tick.finished;
      char c = ' ';
      if (arrived && !done) {
        c = running ? '#' : '_';
      }
      if (finished) {
        done = true;
      }
      out += c;
    }
    out += '\n';
  }

  char buf[128];
  std::snprintf(buf, sizeof buf,
                "\nAverage waiting time:  %.2f cycles\n"
                "Average response time: %.2f cycles\n"
                "Throughput (first %zu cycles): %.2f processes/cycle\n",
                average_waiting_time(), average_response_time(),
                THROUGHPUT_CYCLES, throughput(0, THROUGHPUT_CYCLES));
  out += buf;
  return out;
}
