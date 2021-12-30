
# include other SDC.
#read_sdc SRAM.sdc

# create clocks.
create_clock -period 8.000ns [get_ports CLOCK_125_p]
create_clock -period 20.000ns [get_ports CLOCK_50_B5B]
create_clock -period 20.000ns [get_ports CLOCK_50_B6A]
create_clock -period 20.000ns [get_ports CLOCK_50_B7A]
create_clock -period 20.000ns [get_ports CLOCK_50_B8A]

#create_generated_clock -divide_by 5000 -source CLOCK_50_B5B -name { UART:uart|UART_RX:rx|ClockDivider:rx_clock|o_clock }
#create_generated_clock -divide_by 300 -source CLOCK_50_B5B -name { UART:uart|UART_RT:tx|ClockDivider:tx_clock|o_clock }

#create_clock -name {UART:uart|UART_RX:rx|ClockDivider:rx_clock|o_clock} -period 6250.000 -waveform {0.000 3125.000} { UART:uart|UART_RX:rx|ClockDivider:rx_clock|o_clock }
#create_clock -name {UART:uart|UART_TX:tx|ClockDivider:tx_clock|o_clock} -period 100000.000 -waveform {0.000 50000.000} { UART:uart|UART_RT:tx|ClockDivider:tx_clock|o_clock }

# create derived clocks.
derive_pll_clocks
#derive_clocks -period "1 MHz"

# derive clock uncertainty.
derive_clock_uncertainty

# generate SRAM timing.
#timing_sram CLOCK_50_B5B
