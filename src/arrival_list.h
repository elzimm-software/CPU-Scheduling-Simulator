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
  size_t tot_size = 0;
  std::vector<id_arrival_t> list;

public:
  ArrivalList(); // : real_idx(0), time_idx(0) {}
  bool insert(int pid, int arrival);
  int next();
};

#endif // ARRIVAL_LIST_H_
