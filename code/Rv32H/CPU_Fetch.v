module CPU_Fetch(
	input wire i_reset,
	input wire i_clock,
	input wire i_stall,

	// Bus
	output wire o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,

	// Input
	input wire [7:0] i_tag,
	input wire [31:0] i_pc_next,

	// Output
	output reg [7:0] o_tag,
	output reg [31:0] o_instruction,
	output reg [31:0] o_pc
);

	reg [2:0] state;
	reg [31:0] pc;
	reg [7:0] tag;


	reg icache_request = 0;
	reg [31:0] icache_address;
	wire [31:0] icache_rdata;
	wire icache_ready;

	CPU_ICache icache(
		.i_clock(i_clock),
		.i_request(icache_request),
		.i_address(icache_address),
		.o_rdata(icache_rdata),
		.o_ready(icache_ready),

		.o_bus_request(o_bus_request),
		.o_bus_address(o_bus_address),
		.i_bus_rdata(i_bus_rdata),
		.i_bus_ready(i_bus_ready)
	);

	// 
	`define INSTRUCTION i_bus_rdata
	`include "Instructions_i.v"

	initial begin
		state <= 0;
		pc <= 0;
		tag <= 0;	
		icache_request <= 0;
		o_tag <= 0;
		o_instruction <= 0;
		o_pc <= 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			state <= 0;
			pc <= 0;
			tag <= 0;
			icache_request <= 0;
			o_tag <= 0;
			o_instruction <= 0;
			o_pc <= 0;
		end
		else begin
			case (state)
				0: begin
					if (!i_stall) begin
						icache_address <= pc;
						icache_request <= 1;
						state <= 1;
					end
				end

				1: begin
					if (icache_ready) begin
						o_tag <= tag + 8'd1;
						o_instruction <= icache_rdata;
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

						icache_request <= 0;
					end
				end

				2: begin
					// Wait for branch result.
					if (i_tag == tag) begin
						// $display("fetch, accepting branch to %x", i_pc_next);
						pc <= i_pc_next;
						state <= 0;
					end
				end
			endcase
		end
	end

endmodule
