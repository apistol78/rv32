module CPU_Fetch(
	input wire i_reset,
	input wire i_clock,

	// Bus
	output reg o_bus_request,
	output wire i_bus_ready,
	output reg [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,

	// Input
	input wire i_branch,
	input wire [31:0] i_pc_next,

	// Output
	output reg [31:0] o_instruction,
	output reg [31:0] o_pc,
	output reg o_ready
);

	reg [2:0] state = 0;
	reg [31:0] pc = 0;

	// 
	`define INSTRUCTION i_bus_rdata
	`include "Instructions_i.v"

	initial begin
		o_bus_request <= 0;
		o_bus_address <= 0;
		o_instruction <= 0;
		o_pc <= 0;
		o_ready <= 0;
	end

	always @(posedge i_clock) begin
		if (i_branch) begin
			$display("fetch, accepting branch to %x", i_pc_next);
			pc <= i_pc_next;
			o_bus_address <= i_pc_next;
			o_bus_request <= 1;
			o_ready <= 0;
			state <= 1;
		end
		else begin
			
			case (state)
				0: begin
					$display("fetch %x", pc);
					o_bus_address <= pc;
					o_bus_request <= 1;
					o_ready <= 0;
					state <= 1;
				end

				1: begin
					o_bus_request <= 0;
					
					if (i_bus_ready) begin
						o_instruction <= i_bus_rdata;
						
						o_ready <= 1;
						
						pc <= pc + 4;
						o_pc <= pc;

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
					o_ready <= 0;
				end

			endcase

		end
	end

endmodule
