#include <algorithm>
#include <stdexcept>
#include "arrival_list.h"

void ArrivalList::sort() {
  std::stable_sort(list.begin(), list.end(),
                   [](const id_arrival_t& a, const id_arrival_t& b) {
                     return a.arrival < b.arrival;
                   });
}

void ArrivalList::insert(const int pid, const int arrival) {
  if (running) {
    throw std::logic_error("insert called after next()");
  }
  list.push_back({.pid = pid, .arrival = arrival});
  unsorted = true;
}

int ArrivalList::next() {
  running = true;
  if (unsorted) {
    sort();
    unsorted = false;
  }
  int curr_pid = -1;
  if (real_idx < list.size() &&
      list[real_idx].arrival <= static_cast<int>(time_idx)) {
    curr_pid = list[real_idx].pid;
    real_idx += 1;
  }
  time_idx += 1;
  return curr_pid;
}
