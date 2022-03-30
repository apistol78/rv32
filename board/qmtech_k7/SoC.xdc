# PMOD, pin 1
set_property IOSTANDARD LVCMOS33 [get_ports {uart_rx}]
set_property PACKAGE_PIN D20 [get_ports {uart_rx}]

# PMOD, pin 2
set_property IOSTANDARD LVCMOS33 [get_ports {uart_tx}]
set_property PACKAGE_PIN A24 [get_ports {uart_tx}]

set_property IOSTANDARD LVCMOS33 [get_ports {led_1}]
set_property PACKAGE_PIN B14 [get_ports {led_1}]
set_property IOSTANDARD LVCMOS33 [get_ports {led_2}]
set_property PACKAGE_PIN C14 [get_ports {led_2}]
set_property IOSTANDARD LVCMOS33 [get_ports {led_3}]
set_property PACKAGE_PIN E25 [get_ports {led_3}]

set_property IOSTANDARD LVCMOS18 [get_ports sys_rst]
set_property PACKAGE_PIN AB26 [get_ports sys_rst]

set_property IOSTANDARD LVCMOS33 [get_ports sys_clk]
set_property PACKAGE_PIN F22 [get_ports sys_clk]

