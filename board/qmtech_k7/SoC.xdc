# PMOD, pin 1
set_property IOSTANDARD LVCMOS33 [get_ports {uart_rx}]
set_property PACKAGE_PIN Y22 [get_ports {uart_rx}]

# PMOD, pin 2
set_property IOSTANDARD LVCMOS33 [get_ports {uart_tx}]
set_property PACKAGE_PIN Y23 [get_ports {uart_tx}]

#set_property IOSTANDARD LVCMOS33 [get_ports {led_1}]
#set_property PACKAGE_PIN B14 [get_ports {led_1}]
#set_property IOSTANDARD LVCMOS33 [get_ports {led_2}]
#set_property PACKAGE_PIN C14 [get_ports {led_2}]
#set_property IOSTANDARD LVCMOS33 [get_ports {led_3}]
#set_property PACKAGE_PIN E25 [get_ports {led_3}]

set_property IOSTANDARD LVCMOS33 [get_ports sys_rst]
set_property PACKAGE_PIN AB26 [get_ports sys_rst]

set_property IOSTANDARD LVCMOS33 [get_ports sys_clk]
set_property PACKAGE_PIN F22 [get_ports sys_clk]

# LCD
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_bkl}]
set_property PACKAGE_PIN D19 [get_ports {lcd_bkl}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_clk}]
set_property PACKAGE_PIN C19 [get_ports {lcd_clk}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_de}]
set_property PACKAGE_PIN C17 [get_ports {lcd_de}]


set_property IOSTANDARD LVCMOS33 [get_ports {lcd_r[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_r[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_r[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_r[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_r[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_r[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_r[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_r[7]}]

set_property IOSTANDARD LVCMOS33 [get_ports {lcd_g[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_g[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_g[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_g[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_g[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_g[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_g[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_g[7]}]

set_property IOSTANDARD LVCMOS33 [get_ports {lcd_b[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_b[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_b[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_b[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_b[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_b[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_b[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd_b[7]}]


set_property PACKAGE_PIN A8 [get_ports {lcd_r[0]}]
set_property PACKAGE_PIN A9 [get_ports {lcd_r[1]}]
set_property PACKAGE_PIN B9 [get_ports {lcd_r[2]}]
set_property PACKAGE_PIN C9 [get_ports {lcd_r[3]}]
set_property PACKAGE_PIN A10 [get_ports {lcd_r[4]}]
set_property PACKAGE_PIN B10 [get_ports {lcd_r[5]}]
set_property PACKAGE_PIN D10 [get_ports {lcd_r[6]}]
set_property PACKAGE_PIN E10 [get_ports {lcd_r[7]}]

set_property PACKAGE_PIN C11 [get_ports {lcd_g[0]}]
set_property PACKAGE_PIN C12 [get_ports {lcd_g[1]}]
set_property PACKAGE_PIN A12 [get_ports {lcd_g[2]}]
set_property PACKAGE_PIN A13 [get_ports {lcd_g[3]}]
set_property PACKAGE_PIN D13 [get_ports {lcd_g[4]}]
set_property PACKAGE_PIN D14 [get_ports {lcd_g[5]}]
set_property PACKAGE_PIN A14 [get_ports {lcd_g[6]}]
set_property PACKAGE_PIN B14 [get_ports {lcd_g[7]}]

set_property PACKAGE_PIN A15 [get_ports {lcd_b[0]}]
set_property PACKAGE_PIN B15 [get_ports {lcd_b[1]}]
set_property PACKAGE_PIN D16 [get_ports {lcd_b[2]}]
set_property PACKAGE_PIN D15 [get_ports {lcd_b[3]}]
set_property PACKAGE_PIN B16 [get_ports {lcd_b[4]}]
set_property PACKAGE_PIN C16 [get_ports {lcd_b[5]}]
set_property PACKAGE_PIN A17 [get_ports {lcd_b[6]}]
set_property PACKAGE_PIN B17 [get_ports {lcd_b[7]}]
