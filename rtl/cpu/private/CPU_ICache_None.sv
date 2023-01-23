`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_ICache_None(
	input i_reset,
	input i_clock,
	
	input [31:0] i_input_pc,
	output bit [31:0] o_rdata,
	output bit o_ready,
	input i_stall,

	// Bus
	output bit o_bus_request,
	input i_bus_ready,
	output [31:0] o_bus_address,
	input [31:0] i_bus_rdata,

	// Debug
	output [31:0] o_hit,
	output [31:0] o_miss
);

	typedef enum bit [1:0]
	{
		IDLE		= 2'd0,
		READ_BUS	= 2'd1
	} state_t;

	state_t next = IDLE;
	state_t state = IDLE;

	// Debug, only for verilated.
	assign o_hit = 0;
	assign o_miss = 0;

	assign o_bus_address = i_input_pc;
	
	always_comb begin
		next = state;
	
		o_ready = 0;
		o_rdata = 32'h0;
		o_bus_request = 0;

		case (state)
			IDLE: begin
				if (!i_stall) begin
					o_bus_request = 1;
					next = READ_BUS;
				end
			end

			READ_BUS: begin
				o_bus_request = 1;
				if (i_bus_ready) begin
					o_ready = 1;
					o_rdata = i_bus_rdata;
					next = IDLE;
				end
			end
		endcase

		if (i_reset) begin
			next = IDLE;
		end
	end

endmodule
