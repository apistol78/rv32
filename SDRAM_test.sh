


iverilog -g2005-sv ./rtl/peripherials/SDRAM_controller_tb.sv ./rtl/peripherials/SDRAM_controller.sv ./ip/mt48lc16m16a2.v ./board/qmtech_cv/ip/wbsdram.v ./board/qmtech_cv/rtl/SDRAM_interface_2.sv -s SDRAM_controller_tb -o SDRAM_controller_tb.vvp
vvp SDRAM_controller_tb.vvp
