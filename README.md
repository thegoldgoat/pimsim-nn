# pimsim-nn

Pimsim-nn is a simulator designed for RRAM-based neural network accelerator, which simulates performance and power consumption under given architecture configuration and program instructions.


## Usage

### Requirements

- cmake >= 3.6
- gcc >= 4.8.5

### Build

Cmake is used to build the whole project, run codes below:

```shell
cd pimsim-nn
mkdir build && cd build 
cmake ..
make 
```

In `build` directory, checkout executable file `ChipTest`.

### Simulation Example

There is a built-in resnet-18 example. Configuration and instructions file is under folder `test/resnet18`. Use codes below to simulate resnet-18:

```shell
ChipTest ~/pimsim-nn/test/resnet18/full.gz ~/pimsim-nn/test/resnet18/config.json
```
outputs:
```shell
        SystemC 2.3.4-Accellera --- Jul  4 2023 15:44:33
        Copyright (c) 1996-2022 by all Contributors,
        ALL RIGHTS RESERVED
Loading Inst and Config
Load finish
Reading Inst From Json
hereRead finish
Start Simulation
Progress --- <10%>
Progress --- <20%>
Progress --- <30%>
Progress --- <40%>
Progress --- <50%>
Progress --- <60%>
Progress --- <70%>
Progress --- <80%>
Progress --- <90%>
Simulation Finish
|*************** Simulation Report ***************|
Basic Information:
  - config file:        ../test/resnet18/config.json
  - inst file:          ../test/resnet18/full.gz
  - verbose level:      0
  - core count:         136
  - simulation mode:    0
  - simulation time:    200 ms
Chip Simulation Result:
  - output count:       2.24 samples
  - throughput:         11.2 samples/s
  - average latency:    89.5 ms
  - average power:      6.09e+03 mW
  - average energy:     5.45e+11 pJ/it
```

## Architecture

Pimsim-nn assumes a chip consists of many cores connected via NoC, and the core architecture is shown below:

![image-20230704172641128](https://s2.loli.net/2023/07/04/Y9ZeKzpTORIiakJ.png)

The architecture of core is very similar to a RISC processor, but with four dedicated execute units, namely Scalar Unit, Vector Unit, Matrix Unit and Transfer Unit. **Scalar Unit** is used to process scalar operations. **Vector Unit** performs vector-vector operations. **Matrix Unit** is mainly composed of RRAM crossbar arrays and executes matrix-vector multiply efficiently. **Transfer Unit** is responsible for inter-core data exchange and synchronization. 

## Simulator Inputs

Simulator requires three files:
- Architecture Configuration file 
- NoC Configuration file 
- Program Instructions file

The architecture configuration file primarily defines the latency and power of different components in the simulator. The NoC configuration file gives the latency and power of NoC. Actually, NoC configuration is a part of the architecture configuration, but is separated as an independent file due to the large number of configuration parameters it requires. For simplicity, there is a parameter in architecture configuration that indicates the path of NoC configuration file and simulator would load NoC configuration automatically. The program Instructions are generated by compiler.

Finally, only two inputs are required: one is the path of program instructions file, and the other is the path of architecture configuration file.   


``` shell
ChipTest  path_to_program_instructions_file  path_to_archtecture_configuration_file 
```

There are some parameters in architecture configuration file to change simulation behavior.

| Parameter            | Description                                                  |
| -------------------- | ------------------------------------------------------------ |
| sim_time             | `sim_time` represents simulation time in unit `ms`           |
| sim_mode             | When set to `0`, simulator assumes enough input samples and reports throughout rate. When set to `1`,  simulator will only process one input sample and gives its latency. |
| report_verbose_level | When set to `0`, simulator will only give chip level performance and power consumption statistics. When set to `1`, simulator will also give core level statistics. |

## Authors
- [Xinyu Wang](wangxinyu22s@ict.ac.cn) (Institute of Computing Technology,CAS)

## Acknowledgement
- [systemc](https://github.com/accellera-official/systemc)
- [fmt](https://github.com/fmtlib/fmt)
- [zlib](https://github.com/madler/zlib)
- [nlohmann/json](https://github.com/nlohmann/json)
- [better-enums](https://github.com/aantron/better-enums)
- [filesystem](https://github.com/gulrak/filesystem)
- [zstr](https://github.com/mateidavid/zstr)