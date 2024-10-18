//
// Created by xyfuture on 2023/4/22.
//

#include "chip/Chip.h"
#include "Simulator.h"
#include <fstream>

int sc_main(int argc, char *argv[]) {

  if (argc < 4) {
    std::cout << "Usage: " << argv[0]
              << " <config_file> <inst_file> <is_instruction_in_single_file>"
              << std::endl;
    return 1;
  }
  bool instruction_in_single_file = std::string(argv[3]) == "true";
  Simulator sim(argv[2], argv[1], instruction_in_single_file);
  sim.runSimulation();

  return 0;
}