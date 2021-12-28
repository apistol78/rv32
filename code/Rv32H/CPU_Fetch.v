module CPU_Fetch(
	input wire i_reset,
	input wire i_clock,
	input wire i_stall,

	// Bus
	output reg o_bus_request,
	output wire i_bus_ready,
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

	reg [2:0] state = 0;
	reg [31:0] pc = 0;
	reg [7:0] tag = 0;

	// 
	`define INSTRUCTION i_bus_rdata
	`include "Instructions_i.v"

	initial begin
		o_bus_request <= 0;
		o_bus_address <= 0;
		o_tag <= 0;
		o_instruction <= 0;
		o_pc <= 0;
	end

	always @(posedge i_clock) begin
		case (state)
			0: begin
				if (!i_stall) begin
					$display("fetch %x", pc);
					o_bus_address <= pc;
					o_bus_request <= 1;
					state <= 1;
				end
			end

			1: begin
				if (i_bus_ready) begin

					o_tag <= tag + 1;
					o_instruction <= i_bus_rdata;
					o_bus_request <= 0;
					o_pc <= pc;

					tag <= tag + 1;
					pc <= pc + 4;

					if (!is_BRANCH) begin
						state <= 0;
					end
					else begin
						// Branch instruction, need to wait
						// for an explicit "goto" signal before
						// we can continue feeding the pipeline.
						state <= 2;
					end
				end
			end

			2: begin
				// Wait for branch result.
				if (i_branch) begin
					$display("fetch, accepting branch to %x", i_pc_next);
					pc <= i_pc_next;
					state <= 0;
				end
			end

		endcase
	end

endmodule
