#include <algorithm>
#include "arrival_list.h"

void ArrivalList::sort() {
  std::sort(list.begin(), list.end(),
            [](const id_arrival_t& a, const id_arrival_t& b) {
              return a.arrival < b.arrival;
            });
}

bool ArrivalList::insert(const int pid, const int arrival) {
  if (!list.empty()) {
    const int last_arrival = list.back().arrival;
    if (list.back().arrival == arrival) {
      return false;
    }
    tot_size += arrival - last_arrival;
  }
  tot_size += 1;
  list.push_back({.pid = pid, .arrival = arrival});
  unsorted = true;
  return true;
}

int ArrivalList::next() {
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
