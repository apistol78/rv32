# MOJO Risc-V CPU and System-on-a-Chip
The MOJO SOC is an educational and fun system for FPGA and "Bare metal" programming experimentations.\
It's a RISC-V CPU running at 100 MHz, 32 MiB RAM, audio and 320*200 video resolution.
To make user input easy I've moved USB host functionality to an external PCB using Raspberry Pico as a FPGA <-> HID bridge.

# Prerequisites
This project require an install of Traktor to generate required resources.
You will also need a RISC-V GCC toolchain compiled and installed at __/opt/riscv32/bin__ on your machine.


# How to build
Run __build.sh__ to build host (i.e. Linux) and target products:
```
./build.sh
```
After everything has been built you should be able to run the emulators:
```
./build/linux/ReleaseStatic/Rv32T -elf=./build/rv32/ReleaseStatic/Boot
```
or
```
./build/linux/ReleaseStatic/Rv32 -elf=./build/rv32/ReleaseStatic/Boot
```

# CPU
MOJO CPU is a conventional 5-stage (F,D,X,M,W) pipelined RISC-V CPU. It also contain configurable data- and instruction caches.

Written using SystemVerilog and are split up into logical pieces where each stage of the pipeline is in a separate module:

| Module | Description |
|--------|-------------|
| CPU_ALU | ALU unit, fully combinatorial |
| CPU_CSR | CSR control unit |
| CPU_DCache_Comb | Data cache, combinatorial |
| CPU_DCache_Reg | Data cache, registered |
| CPU_DCache_None | Data cache, disabled |
| CPU_Decode | Decode stage |
| CPU_Defines | CPU defines |
| CPU_Divide | Divison wrapper, usually implemented with a vendor IP |
| CPU_Execute | Execute stage |
| CPU_Fetch | Fetch stage |
| CPU_Forward | Register forwarding logic |
| CPU_ICache_Comb | Instruction cache, combinatorial |
| CPU_ICache_Reg | Instruction cache, registered |
| CPU_ICache_None | Instruction cache, disabled |
| CPU_Memory | Memory stage |
| CPU_Multiply | Multiplication wrapper, usually implemented with a vendor IP |
| CPU_Registers | Register file |
| CPU_SkidBuffer | Generalized skid-buffer |
| CPU_Types | CPU types |
| CPU_Writeback | Writeback stage |

# Perifherials
A lot of perifherial modules has been implemented as port of the SoC:
| Module | Description |
|--------|-------------|
| AUDIO_controller, AUDIO_i2s_output, AUDIO_pwm_output | PCM based audio modules |
| BRAM | Various block-ram modules |
| BRIDGE | BUS bridge, to separate BUS devices for improved timing. |
| BROM | Block-rom module |
| BusAccess | BUS multiplexed access |
| ClockDivider | Generic clock divider |
| DMA | Direct memory access controller |
| FIFO_BRAM | Block ram backed FIFO |
| FIFO | Register backed FIFO |
| GPIO | General purpose IO controller |
| I2C | I2C controller |
| LED | LED controller |
| PLIC | PLIC interrupt controller |
| RESET | Reset controller |
| SD | SD controller |
| SDRAM_controller | Generic SDRAM controller |
| SRAM_interface | Generic SRAM controller |
| SystemRegisters | System wide register map |
| Timer | Hardware timer with compare |
| UART | UART controller |
| VIDEO_controller | Video output controller |
| VIDEO_VGA | Video VGA signal generator |
| WriteBuffer | Write buffer (queue) with readback or read-through |

# System-on-a-Chip
A couple of custom PCBs designed to connect to QM-Tech FPGA boards is the physical basis for MOJO SoC.
The SoC support following perifherials:
* HDMI output
* Stereo audio output through PWM and through I2S HDMI.
* SD card interface using 4-bit SD protocol.
* 2 UARTs, one for host and one for HID input.
* Raspberry Pico based USB host to FPGA bridge supporting three external HID devices (keyboard, mouse and gamepad).

## Emulators
Two different emulators, one C++ highlevel emulator and one based on Verilator. The highlevel is useful for application development and the other is used for verification of gate level logic.

## Runtime
C/C++ runtime library, including a preemptive multithreaded kernel, expose all hardware features to user programs. Support CRT for easy portability.

## Schematics
KiCad schematics for all perifherial boards are available and been built using JLCPCB.
