
# create clocks.
create_clock -period 20.000ns [get_ports sys_clk]

# create derived clocks.
derive_pll_clocks

# derive clock uncertainty.
derive_clock_uncertainty
