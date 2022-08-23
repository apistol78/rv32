
# create clocks.
create_clock -period 20.000ns [get_ports sys_clk]

# create derived clocks.
derive_pll_clocks

# derive clock uncertainty.
derive_clock_uncertainty

# create async clock groups from system clock and video clock.
set_clock_groups -asynchronous -group [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}] -group [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[2].gpll~PLL_OUTPUT_COUNTER|divclk}]

# register forward path, suspect it's a false path since we assume result from last cycle.
set_false_path -from {CPU:cpu|CPU_Execute:execute|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Memory:memory|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Decode:decode|data.inst_rs1[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Decode:decode|data.inst_rs2[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}
set_false_path -from {CPU:cpu|CPU_Writeback:writeback|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|data.rd[*]}

set_false_path -from {CPU:cpu|CPU_Execute:execute|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|o_jump_pc[*]}
set_false_path -from {CPU:cpu|CPU_Memory:memory|data.inst_rd[*]} -to {CPU:cpu|CPU_Execute:execute|o_jump_pc[*]}
set_false_path -from {CPU:cpu|CPU_Writeback:writeback|data.inst_rd[*]~DUPLICATE} -to {CPU:cpu|CPU_Execute:execute|o_jump_pc[*]}
