#include "execution_history.h"

#include <algorithm>
#include <cstdio>

HistoryEvent::HistoryEvent(const int pid, const EventType event) {
  this->pid = pid;
  this->event = event;
}

int HistoryEvent::get_pid() const { return pid; }

EventType HistoryEvent::get_event() const { return event; }

void ExecutionHistory::add_proc(const int pid, const std::string &name) {
  names[pid] = name;
}

void ExecutionHistory::add_event(int pid, EventType event) {
  history.back().emplace_back(pid, event);
}

/**
 * Starts a new tick. Must be called before the first add_event of each tick.
 **/
void ExecutionHistory::step() {
  history.emplace_back();
}

bool ExecutionHistory::has_event(const size_t tick, const int pid,
                                 const EventType type) const {
  return std::any_of(history[tick].begin(), history[tick].end(),
                     [&](const HistoryEvent &e) {
                       return e.get_pid() == pid && e.get_event() == type;
                     });
}

// FINISHED implies the process also ran during that tick.
bool ExecutionHistory::is_running(const size_t tick, const int pid) const {
  return has_event(tick, pid, RUNNING) || has_event(tick, pid, FINISHED);
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
    for (size_t t = 0; t < history.size(); t++) {
      arrived = arrived || has_event(t, pid, ARRIVED);
      if (!arrived) {
        continue;
      }
      if (!is_running(t, pid)) {
        total++;
      }
      if (has_event(t, pid, FINISHED)) {
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
      if (!arrived && has_event(t, pid, ARRIVED)) {
        arrived = true;
        arrival = t;
      }
      if (arrived && is_running(t, pid)) {
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
    for (const auto &e : history[t]) {
      if (e.get_event() == FINISHED) {
        finished++;
      }
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
      bool running = false;
      bool finished = false;
      for (const auto &e : tick) {
        if (e.get_pid() != pid) {
          continue;
        }
        switch (e.get_event()) {
        case ARRIVED:
          arrived = true;
          break;
        case RUNNING:
          running = true;
          break;
        case FINISHED:
          running = true;
          finished = true;
          break;
        }
      }
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
