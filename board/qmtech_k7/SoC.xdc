
# create async clock groups from system clock and video clock.
#set_clock_groups -asynchronous -group [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}] -group [get_clocks {pll_clk|ip_pll_clk_inst|altera_pll_i|general[2].gpll~PLL_OUTPUT_COUNTER|divclk}]
set_false_path -from [get_clocks -of_objects [get_pins ipclk/inst/mmcm_adv_inst/CLKOUT2]] -to [get_clocks -of_objects [get_pins ipclk/inst/mmcm_adv_inst/CLKOUT0]]

# UART FTDI
set_property IOSTANDARD LVCMOS33 [get_ports {uart_0_rx}]
set_property PACKAGE_PIN Y23 [get_ports {uart_0_rx}]
set_property IOSTANDARD LVCMOS33 [get_ports {uart_0_tx}]
set_property PACKAGE_PIN Y22 [get_ports {uart_0_tx}]

# UART IO
set_property IOSTANDARD LVCMOS33 [get_ports {uart_1_rx}]
set_property PACKAGE_PIN B26 [get_ports {uart_1_rx}]
set_property IOSTANDARD LVCMOS33 [get_ports {uart_1_tx}]
set_property PACKAGE_PIN C26 [get_ports {uart_1_tx}]

# SD
set_property IOSTANDARD LVCMOS33 [get_ports {sd_clk}]
set_property PACKAGE_PIN AE22 [get_ports {sd_clk}]
set_property IOSTANDARD LVCMOS33 [get_ports {sd_cmd}]
set_property PACKAGE_PIN AE23 [get_ports {sd_cmd}]
set_property IOSTANDARD LVCMOS33 [get_ports {sd_dat[0]}]
set_property PACKAGE_PIN AA25 [get_ports {sd_dat[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {sd_dat[1]}]
set_property PACKAGE_PIN AC23 [get_ports {sd_dat[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {sd_dat[2]}]
set_property PACKAGE_PIN AD26 [get_ports {sd_dat[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {sd_dat[3]}]
set_property PACKAGE_PIN AF24 [get_ports {sd_dat[3]}]

# I2S
set_property IOSTANDARD LVCMOS33 [get_ports {i2s_sdout}]
set_property PACKAGE_PIN AA22 [get_ports {i2s_sdout}]
set_property IOSTANDARD LVCMOS33 [get_ports {i2s_sclk}]
set_property PACKAGE_PIN AA24 [get_ports {i2s_sclk}]
set_property IOSTANDARD LVCMOS33 [get_ports {i2s_lrck}]
set_property PACKAGE_PIN AC24 [get_ports {i2s_lrck}]
set_property IOSTANDARD LVCMOS33 [get_ports {i2s_mclk}]
set_property PACKAGE_PIN AB25 [get_ports {i2s_mclk}]


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

set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_nreset}]
set_property PACKAGE_PIN C9 [get_ports {hdmi_nreset}]

set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_sda}]
set_property PACKAGE_PIN D10 [get_ports {hdmi_sda}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_scl}]
set_property PACKAGE_PIN E10 [get_ports {hdmi_scl}]


set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_idck}]
set_property PACKAGE_PIN D16 [get_ports {hdmi_idck}]

set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_vsync}]
set_property PACKAGE_PIN B19 [get_ports {hdmi_vsync}]

set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_hsync}]
set_property PACKAGE_PIN C19 [get_ports {hdmi_hsync}]

set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_int}]
set_property PACKAGE_PIN A23 [get_ports {hdmi_int}]

set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_de}]
set_property PACKAGE_PIN A19 [get_ports {hdmi_de}]

set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_sdo}]
set_property PACKAGE_PIN A20 [get_ports {hdmi_sdo}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_sck}]
set_property PACKAGE_PIN D20 [get_ports {hdmi_sck}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_mclk}]
set_property PACKAGE_PIN B20 [get_ports {hdmi_mclk}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_ws}]
set_property PACKAGE_PIN D19 [get_ports {hdmi_ws}]


set_property PACKAGE_PIN A18 [get_ports {hdmi_r[0]}]
set_property PACKAGE_PIN C18 [get_ports {hdmi_r[1]}]
set_property PACKAGE_PIN C17 [get_ports {hdmi_r[2]}]
set_property PACKAGE_PIN D18 [get_ports {hdmi_r[3]}]
set_property PACKAGE_PIN E18 [get_ports {hdmi_r[4]}]
set_property PACKAGE_PIN A17 [get_ports {hdmi_r[5]}]
set_property PACKAGE_PIN B17 [get_ports {hdmi_r[6]}]
set_property PACKAGE_PIN B16 [get_ports {hdmi_r[7]}]

set_property PACKAGE_PIN C16 [get_ports {hdmi_g[0]}]
set_property PACKAGE_PIN D15 [get_ports {hdmi_g[1]}]
set_property PACKAGE_PIN A15 [get_ports {hdmi_g[2]}]
set_property PACKAGE_PIN B15 [get_ports {hdmi_g[3]}]
set_property PACKAGE_PIN C13 [get_ports {hdmi_g[4]}]
set_property PACKAGE_PIN C14 [get_ports {hdmi_g[5]}]
set_property PACKAGE_PIN A14 [get_ports {hdmi_g[6]}]
set_property PACKAGE_PIN B14 [get_ports {hdmi_g[7]}]

set_property PACKAGE_PIN D13 [get_ports {hdmi_b[0]}]
set_property PACKAGE_PIN D14 [get_ports {hdmi_b[1]}]
set_property PACKAGE_PIN A12 [get_ports {hdmi_b[2]}]
set_property PACKAGE_PIN A13 [get_ports {hdmi_b[3]}]
set_property PACKAGE_PIN C11 [get_ports {hdmi_b[4]}]
set_property PACKAGE_PIN C12 [get_ports {hdmi_b[5]}]
set_property PACKAGE_PIN B11 [get_ports {hdmi_b[6]}]
set_property PACKAGE_PIN B12 [get_ports {hdmi_b[7]}]

set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_r[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_r[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_r[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_r[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_r[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_r[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_r[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_r[7]}]

set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_g[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_g[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_g[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_g[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_g[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_g[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_g[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_g[7]}]

set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_b[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_b[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_b[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_b[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_b[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_b[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_b[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {hdmi_b[7]}]

