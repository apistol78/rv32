


iverilog -g2005-sv ../rtl/peripherials/SRAM_interface_tb.sv ../rtl/peripherials/SRAM_interface.sv ../ip/sram_model.v -s SRAM_interface_tb -o SRAM_interface_tb.vvp
vvp SRAM_interface_tb.vvp
