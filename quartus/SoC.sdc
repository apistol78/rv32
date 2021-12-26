
# include other SDC.
read_sdc SRAM.sdc

# create clocks.
create_clock -period 8.000ns [get_ports CLOCK_125_p]
create_clock -period 20.000ns [get_ports CLOCK_50_B5B]
create_clock -period 20.000ns [get_ports CLOCK_50_B6A]
create_clock -period 20.000ns [get_ports CLOCK_50_B7A]
create_clock -period 20.000ns [get_ports CLOCK_50_B8A]

# create derived clocks.
derive_pll_clocks
derive_clocks -period "1 MHz"

# derive clock uncertainty.
derive_clock_uncertainty

# generate SRAM timing.
timing_sram CLOCK_50_B5B
