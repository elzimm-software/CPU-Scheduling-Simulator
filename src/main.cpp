/**
 * Jurassic CPU Scheduler Simulator
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>

#include "cpu_sim.h"
#include "fifo.h"
#include "round_robin.h"
#include "sjf.h"

constexpr char USAGE[] = "USAGE disched PATH";

static int read_file(const std::string& path, std::vector<CPUSim::proc_input_t>& procs) {
  std::ifstream f(path);
  if (!f.is_open()) {
    std::cerr << "failed to open file";
    return 1;
  }
  std::string proc_line;
  while (std::getline(f, proc_line)) {
    if (proc_line.find_first_not_of(" \t\r\n\f\v") == std::string::npos) {
      continue;
    }
    std::istringstream iss(proc_line);
    CPUSim::proc_input_t proc;
    if (!(iss >> proc.realname >> proc.burst >> proc.arrival)) {
      std::cerr << "malformed proc line: " << proc_line << "\n";
      return 1;
    }
    if (proc.burst <= 0) {
      std::cerr << "non-positive burst: " << proc_line << "\n";
      return 1;
    }
    if (proc.arrival < 0) {
      std::cerr << "negative arrival: " << proc_line << "\n";
      return 1;
    }
    procs.push_back(proc);
  }
  f.close();
  return 0;
}

template<class T>
static void run_and_display(const std::string& name, const std::vector<CPUSim::proc_input_t>& procs) {
  static_assert(std::is_base_of_v<Scheduler, T>);
  CPUSim sim(std::make_unique<T>(), procs);
  sim.run();
  std::cout << name << "\n" << sim.format_history() << "\n";
}

int main(const int argc, char** argv) {
  if (argc != 2) {
    std::cout << USAGE << "\n";
    return 1;
  }
  std::vector<CPUSim::proc_input_t> procs;
  if (read_file(argv[1], procs)) {
    return 1;
  }

  run_and_display<FiFo>("FiFo", procs);
  run_and_display<SJF>("SJF", procs);
  run_and_display<RoundRobin>("Round Robin", procs);

  return 0;
}
