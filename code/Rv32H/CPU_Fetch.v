module CPU_Fetch(
	input wire i_reset,
	input wire i_clock,
	input wire i_stall,

	// Bus
	output reg o_bus_request,
	input wire i_bus_ready,
	output reg [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,

	// Input
	input wire i_branch,
	input wire [31:0] i_pc_next,

	// Output
	output reg [7:0] o_tag,
	output reg [31:0] o_instruction,
	output reg [31:0] o_pc
);

	reg [2:0] state;
	reg [31:0] pc;
	reg [7:0] tag;

	// 
	`define INSTRUCTION i_bus_rdata
	`include "Instructions_i.v"

	initial begin
		state <= 0;
		pc <= 0;
		tag <= 0;	
		o_bus_address <= 0;
		o_bus_request <= 0;
		o_tag <= 0;
		o_instruction <= 0;
		o_pc <= 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			state <= 0;
			pc <= 0;
			tag <= 0;
			o_bus_address <= 0;
			o_bus_request <= 0;
			o_tag <= 0;
			o_instruction <= 0;
			o_pc <= 0;
		end
		else begin
			case (state)
				0: begin
					if (!i_stall) begin
						o_bus_address <= pc;
						o_bus_request <= 1;
						state <= 1;
					end
				end

				1: begin
					if (i_bus_ready) begin
						o_tag <= tag + 8'd1;
						o_instruction <= i_bus_rdata;
						o_pc <= pc;

						tag <= tag + 8'd1;
						pc <= pc + 4;

						if (is_BRANCH) begin
							// Branch instruction, need to wait
							// for an explicit "goto" signal before
							// we can continue feeding the pipeline.
							state <= 2;
						end
						else
							state <= 0;

						o_bus_request <= 0;
					end
				end

				2: begin
					// Wait for branch result.
					if (i_branch) begin
						// $display("fetch, accepting branch to %x", i_pc_next);
						pc <= i_pc_next;
						state <= 0;
					end
				end
			endcase
		end
	end

endmodule
