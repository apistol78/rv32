
# include other SDC.
#read_sdc SRAM.sdc

# create clocks.
create_clock -period 8.000ns [get_ports CLOCK_125_p]
create_clock -period 20.000ns [get_ports CLOCK_50_B5B]
create_clock -period 20.000ns [get_ports CLOCK_50_B6A]
create_clock -period 20.000ns [get_ports CLOCK_50_B7A]
create_clock -period 20.000ns [get_ports CLOCK_50_B8A]

# create derived clocks.
derive_pll_clocks

# derive clock uncertainty.
derive_clock_uncertainty

# generate SRAM timing.
#timing_sram CLOCK_50_B5B

#**************************************************************
# Set False Path
#**************************************************************
#set_false_path -from [get_clocks CLOCK_50_B5B] -to [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll_config_clk}]
#set_false_path -from [get_clocks CLOCK_50_B5B] -to [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll_afi_half_clk}]
#set_false_path -from [get_clocks CLOCK_50_B5B] -to [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll_avl_clk}]
#set_false_path -from [get_clocks CLOCK_50_B5B] -to [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll_afi_clk}]
#set_false_path -from [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll1~PLL_OUTPUT_COUNTER|divclk}] -to [get_clocks {CLOCK_50_B5B}]

#set_false_path -from [get_clocks {CLOCK_50_B5B}] -to [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll7~PLL_OUTPUT_COUNTER|divclk sdram|sdram|ip_sdram_inst|pll0|pll1~FRACTIONAL_PLL|vcoph[0] sdram|sdram|ip_sdram_inst|pll0|pll1~PLL_OUTPUT_COUNTER|divclk sdram|sdram|ip_sdram_inst|pll0|pll3~PLL_OUTPUT_COUNTER|divclk sdram|sdram|ip_sdram_inst|pll0|pll5~PLL_OUTPUT_COUNTER|divclk sdram|sdram|ip_sdram_inst|pll0|pll6~PLL_OUTPUT_COUNTER|divclk}]
#set_false_path -from [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll1~PLL_OUTPUT_COUNTER|divclk}] -to [get_clocks {CLOCK_50_B5B}]
#set_false_path -from [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll1~PLL_OUTPUT_COUNTER|divclk}] -to [get_clocks {CLOCK_125_p}]

set_false_path -from [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll1~PLL_OUTPUT_COUNTER|divclk}] -to [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}]
set_false_path -from [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll1~PLL_OUTPUT_COUNTER|divclk}] -to [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll6~PLL_OUTPUT_COUNTER|divclk}]

set_false_path -from [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}] -to [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll1~PLL_OUTPUT_COUNTER|divclk}]
set_false_path -from [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}] -to [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll6~PLL_OUTPUT_COUNTER|divclk}]
set_false_path -from [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}] -to [get_clocks {sdram|sdram|ip_sdram_inst|pll0|pll7~PLL_OUTPUT_COUNTER|divclk}]

# register forward path, suspect it's a false path since we assume result from last cycle.
set_false_path -from {CPU:cpu|CPU_Execute:execute|data.inst_rd[*]~DUPLICATE} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Memory:memory|data.inst_rd[*]~DUPLICATE} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Decode:decode|data.inst_rs[*]~DUPLICATE} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Writeback:writeback|data.inst_rd[*]~DUPLICATE} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}

set_false_path -from {CPU:cpu|CPU_Execute:execute|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Memory:memory|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Decode:decode|data.inst_rs1[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Decode:decode|data.inst_rs2[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Writeback:writeback|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}

#**************************************************************
# Set Multicycle Path
#**************************************************************
#set_multicycle_path -from {Avalon_bus_RW_Test:fpga_lpddr2_Verify|avl_address*} -to {Avalon_bus_RW_Test:fpga_lpddr2_Verify|avl_writedata*} -setup -end 6
#set_multicycle_path -from {Avalon_bus_RW_Test:fpga_lpddr2_Verify|cal_data*} -to {Avalon_bus_RW_Test:fpga_lpddr2_Verify|avl_writedata*} -setup -end 6

#set_multicycle_path -from {Avalon_bus_RW_Test:fpga_lpddr2_Verify|avl_address*} -to {Avalon_bus_RW_Test:fpga_lpddr2_Verify|avl_writedata*} -hold -end 6
#set_multicycle_path -from {Avalon_bus_RW_Test:fpga_lpddr2_Verify|cal_data*} -to {Avalon_bus_RW_Test:fpga_lpddr2_Verify|avl_writedata*} -hold -end 6

#set_multicycle_path -from {CPU_v2:cpu|CPU_Decode:decode|o_inst_rd*} -to {CPU_v2:cpu|CPU_Execute:execute|o_rd*]} -setup -end 1
#set_multicycle_path -from {CPU_v2:cpu|CPU_Execute:execute|o_inst_rd*} -to {CPU_v2:cpu|CPU_Execute:execute|o_rd*]} -setup -end 1
#set_multicycle_path -from {CPU_v2:cpu|CPU_Memory:memory|o_inst_rd*} -to {CPU_v2:cpu|CPU_Execute:execute|o_rd*]} -setup -end 1

#set_multicycle_path -from {CPU_v2:cpu|CPU_Decode:decode|o_inst_rd*} -to {CPU_v2:cpu|CPU_Execute:execute|o_pc_next*]} -setup -end 1
#set_multicycle_path -from {CPU_v2:cpu|CPU_Memory:memory|o_inst_rd*} -to {CPU_v2:cpu|CPU_Execute:execute|o_pc_next*]} -setup -end 1
#set_multicycle_path -from {CPU_v2:cpu|CPU_Execute:execute|o_inst_rd*} -to {CPU_v2:cpu|CPU_Execute:execute|o_pc_next*]} -setup -end 1
