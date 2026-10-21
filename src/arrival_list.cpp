#include "arrival_list.h"

bool ArrivalList::insert(int pid, int arrival) {
  if (!list.empty()) {
    int last_arrival = list.back().arrival;
    if (list.back().arrival == arrival) {
      return false;
    }
    tot_size += arrival - last_arrival;
  }
  tot_size += 1;
  list.push_back({pid, arrival});
  return true;
}

int ArrivalList::next() {
  int curr_pid = -1;
  if (list[real_idx].arrival <= time_idx) {
    curr_pid = list[real_idx].pid;
    real_idx += 1;
  }
  time_idx += 1;
  return curr_pid;
}
