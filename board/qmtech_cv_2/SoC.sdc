

# create clocks.
create_clock -period 20.000ns [get_ports sys_clk]

# Allow refer to SDRAM clock by name.
create_generated_clock -name vclk_sdram -source [get_pins {pll_clk|ip_pll_clk_inst|altera_pll_i|general[1].gpll~PLL_OUTPUT_COUNTER|divclk}] [get_ports {sdram_clk}]
create_generated_clock -name vclk_sram -source [get_pins {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}]

# create derived clocks.
derive_pll_clocks

# derive clock uncertainty.
derive_clock_uncertainty

#####################################################################################

# Set input delay.
#  (data from SDRAM chip into FPGA)

# max output time 6.0 + trace delay 0.25
set_input_delay -clock vclk_sdram -max 6.25 [get_ports sdram_data*]    
# min output time 0.0 + trace delay 0.25
set_input_delay -clock vclk_sdram -min 0.25 [get_ports sdram_data*]    

# Set output delay.
#  (data from FPGA to SDRAM chip)

# input setup time
set_output_delay -clock vclk_sdram -max 1.5 [get_ports sdram_*]
# input hold time      
set_output_delay -clock vclk_sdram -min -1.0 [get_ports sdram_*]      

set_multicycle_path -from [get_clocks {vclk_sdram}] -to [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}] -setup -end 2

# clock domain crossing
set_false_path -from {sdram_data*} -to {SDRAM_controller:sdram|r_ram_data_ext_clk[*]}

#####################################################################################

# Tri-state bridge timing
set sram_tperWr     10
set sram_tperRd     20

# SRAM Address Access Time (tAA)
set sram_taa       10.0

# PCB delay
set sram_tpcb       0.1

# FPGA's IO timing
# -> tco depends on FPGA (refer to www.altera.com IO spreadsheet)
set fpga_tco        7.5
set fpga_tsu        [expr $sram_tperRd - $sram_taa - $fpga_tco - 2*$sram_tpcb]
set fpga_th         0.0
set fpga_tcomin     0.0

set delay_in_max    [expr $sram_tperRd - $fpga_tsu]
set delay_in_min    $fpga_th
set delay_out_max   [expr $sram_tperWr - $fpga_tco]
set delay_out_min   $fpga_tcomin

set_input_delay -clock vclk_sram -max $delay_in_max [get_ports sram_d[*]]
set_input_delay -clock vclk_sram -min $delay_in_min [get_ports sram_d[*]]

set_output_delay -clock vclk_sram -max $delay_out_max [get_ports sram_*]
set_output_delay -clock vclk_sram -min $delay_out_min [get_ports sram_*]

set_multicycle_path -from [get_clocks vclk_sram] -to [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}] -setup -end 2
set_multicycle_path -from [get_clocks vclk_sram] -to [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}] -hold -end 1

set_clock_groups -asynchronous -group [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}] -group [get_clocks vclk_sram]

#####################################################################################

# create async clock groups from system clock and video clock.
set_clock_groups -asynchronous -group [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}] -group [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[2].gpll~PLL_OUTPUT_COUNTER|divclk}]

# register forward path, suspect it's a false path since we assume result from last cycle.
set_false_path -from {CPU:cpu|CPU_Execute:execute|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Memory:memory|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
#set_false_path -from {CPU:cpu|CPU_Decode:decode|data.inst_rs1[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Decode:decode|data.inst_rs2[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Writeback:writeback|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}

set_false_path -from {CPU:cpu|CPU_Execute:execute|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|o_jump_pc[*]}
set_false_path -from {CPU:cpu|CPU_Memory:memory|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|o_jump_pc[*]}
#set_false_path -from {CPU:cpu|CPU_Writeback:writeback|data.inst_rd[*]~DUPLICATE} -to {CPU:cpu|CPU_Execute:execute|o_jump_pc[*]}
