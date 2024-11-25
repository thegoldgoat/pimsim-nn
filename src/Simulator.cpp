//
// Created by xyfuture on 2023/5/2.
//

#include "Simulator.h"
#include "chip/Chip.h"
#include "utils/Timer.h"

#include <utility>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <fmt/core.h>
#include <zstr.hpp>
#include <ghc/filesystem.hpp>
#include <chrono>


//namespace fs = std::filesystem;
namespace fs = ghc::filesystem;

Simulator::Simulator(std::string config_file_path_, std::string inst_file_path_, bool instruction_in_single_file):
        config_file_path(std::move(config_file_path_)), inst_file_path(std::move(inst_file_path_)), instruction_in_single_file(instruction_in_single_file) {}

void Simulator::runSimulation() {

    auto start = std::chrono::high_resolution_clock::now();


    std::ifstream config_file(config_file_path);


    fs::path file_path(config_file_path);
    auto parent_path = file_path.parent_path();

    std::cout<<"Loading Config --- "<<std::endl;

    nlohmann::json json_config = nlohmann::json::parse(config_file);

    std::cout<<"Load config finish"<<std::endl;

    global_config = json_config.get<GlobalConfig>();

    chip_ptr = std::make_shared<Chip>(global_config.chip_config,global_config.sim_config);

    chip_ptr->network.readLatencyEnergyFile(parent_path.string());
    std::cout<<"Read finish"<<std::endl;

    std::cout<<"Reading Instructions From File --- "<<std::endl;

    if (instruction_in_single_file) {
        zstr::ifstream inst_file(inst_file_path,std::ios::binary); // compressed
        nlohmann::json json_inst = nlohmann::json::parse(inst_file);
        chip_ptr->initializeCores(json_inst);
    } else {
        chip_ptr->initializeCoresWithDir(inst_file_path);
    }

    int levels = 10;
    ProgressBar bar(SC_MS,levels,global_config.sim_config.sim_time,[this](int progress){
        std::cout<<fmt::format("Progress --- <{}0%>",progress)<<std::endl;
    });


    std::cout<<"Start Simulation --- "<<std::endl;
    sc_start(global_config.sim_config.sim_time,sc_core::SC_MS);
    std::cout<<"Simulation Finish"<<std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    double executionTime = duration.count();
    std::cout<<"simulator execution time:"<<executionTime<<"s"<<std::endl;
    std::cout<<getSimulationReport()<<std::endl;

}

std::string Simulator::getSimulationReport() {
    std::stringstream  s;

    auto sub_line = "  - {:<20}{}\n";
    s<<"|*************** Simulation Report ***************|\n";
    s<<"Basic Information:\n";
    s<<fmt::format(sub_line, "config file:",config_file_path);
    s<<fmt::format(sub_line, "inst file:",inst_file_path);
    s<<fmt::format(sub_line, "verbose level:", global_config.sim_config.report_verbose_level);
    s<<fmt::format(sub_line, "core count:", global_config.chip_config.core_cnt);
    s<<fmt::format(sub_line, "simulation mode:",global_config.sim_config.sim_mode);

    if (global_config.sim_config.sim_mode == 0)
        s<<fmt::format("  - {:<20}{} ms\n","simulation time:",global_config.sim_config.sim_time);


    return s.str() + chip_ptr->getSimulationReport();
}

void Simulator::progressBar() {
    int progress = (sc_time_stamp().to_seconds() / (global_config.sim_config.sim_time/1000)) * 100;
    std::cout<<fmt::format("Progress --- <{}%>",progress)<<std::endl;
}


std::string Simulator::getBasicInformation() {
    stringstream s;
    auto sub_line = "  - {:<20}{}\n";

    s<<"Basic Information:\n";
    s<<fmt::format(sub_line, "config file:",config_file_path);
    s<<fmt::format(sub_line, "inst file:",inst_file_path);
    s<<fmt::format(sub_line, "verbose level:", global_config.sim_config.report_verbose_level);
    s<<fmt::format(sub_line, "core count:", global_config.chip_config.core_cnt);
    s<<fmt::format(sub_line, "simulation mode:",global_config.sim_config.sim_mode);

    return s.str();
}


