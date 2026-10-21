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

int main(const int argc, char** argv) {
  if (argc != 2) {
    std::cout << USAGE << "\n";
    return 1;
  }
  std::vector<CPUSim::proc_input_t> procs;
  if (read_file(argv[1], procs)) {
    return 1;
  }
  CPUSim sim(std::make_unique<FiFo>(), procs);
  sim.run();
  std::cout << sim.format_history();
  return 0;
}
