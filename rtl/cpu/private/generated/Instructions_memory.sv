// This file is automatically generated, DO NOT EDIT!
// ==================================================

wire memory_read = 
	is_FLW |
	is_LB  |
	is_LBU |
	is_LH  |
	is_LHU |
	is_LW  |
	1'b0;

wire memory_write = 
	is_FSW |
	is_SB  |
	is_SH  |
	is_SW  |
	1'b0;

wire [1:0] memory_width = 
	is_FLW ? 2'b10 :
	is_FSW ? 2'b10 :
	is_LB  ? 2'b00 :
	is_LBU ? 2'b00 :
	is_LH  ? 2'b01 :
	is_LHU ? 2'b01 :
	is_LW  ? 2'b10 :
	is_SB  ? 2'b00 :
	is_SH  ? 2'b01 :
	is_SW  ? 2'b10 :
	3'd0;

wire memory_signed = 
	is_LB |
	is_LH |
	1'b0;

