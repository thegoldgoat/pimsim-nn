//
// Created by xyfuture on 2023/5/2.
//



#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <string>
#include "config/Config.hpp"
#include "chip/Chip.h"

class Simulator {
public:
    Simulator(std::string config_file_path_, std::string inst_file_path_, bool instruction_in_single_file);

    void runSimulation();

    void progressBar();

    std::string getBasicInformation();
    std::string getSimulationReport();

private:
    GlobalConfig global_config;
    std::string config_file_path;
    std::string inst_file_path;

    std::shared_ptr<Chip> chip_ptr;

    bool instruction_in_single_file;
};


#endif //SIMULATOR_H_
