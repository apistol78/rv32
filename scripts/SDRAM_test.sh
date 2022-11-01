
# Get normalized path to game home, excluding file name.
ROOT="`dirname \"$BASH_SOURCE\"`"
ROOT="`(cd \"$ROOT/..\" && pwd)`"

iverilog -g2005-sv $ROOT/rtl/peripherials/SDRAM_controller_tb.sv $ROOT/rtl/peripherials/SDRAM_controller.sv $ROOT/ip/mt48lc16m16a2.v $ROOT/board/qmtech_cv/ip/wbsdram.v $ROOT/board/qmtech_cv/rtl/SDRAM_interface_2.sv -s SDRAM_controller_tb -o $ROOT/SDRAM_controller_tb.vvp
vvp $ROOT/SDRAM_controller_tb.vvp
