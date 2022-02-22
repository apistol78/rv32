set_global_assignment -entity "IP_PLL_Clk" -library "lib_IP_PLL_Clk" -name IP_TOOL_NAME "altera_pll"
set_global_assignment -entity "IP_PLL_Clk" -library "lib_IP_PLL_Clk" -name IP_TOOL_VERSION "21.1"
set_global_assignment -entity "IP_PLL_Clk" -library "lib_IP_PLL_Clk" -name IP_TOOL_ENV "mwpim"
set_global_assignment -library "lib_IP_PLL_Clk" -name SPD_FILE [file join $::quartus(sip_path) "IP_PLL_Clk.spd"]

set_global_assignment -library "lib_IP_PLL_Clk" -name MISC_FILE [file join $::quartus(sip_path) "IP_PLL_Clk_sim/IP_PLL_Clk.vo"]
