#ifndef ARRIVAL_LIST_H_
#define ARRIVAL_LIST_H_

#include <cstddef>
#include <vector>

class ArrivalList {

  struct id_arrival_t {
    int pid;
    int arrival;
  };

  size_t real_idx = 0;
  size_t time_idx = 0;
  bool unsorted = true;
  bool running = false;
  std::vector<id_arrival_t> list;

  void sort();

public:
  ArrivalList() = default; // : real_idx(0), time_idx(0) {}
  void insert(int pid, int arrival);
  int next();
};

#endif // ARRIVAL_LIST_H_
