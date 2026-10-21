#include "execution_history.h"

#include <algorithm>

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
  return out;
}
