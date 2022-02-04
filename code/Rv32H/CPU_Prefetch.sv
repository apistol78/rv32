
`timescale 1ns/1ns

module CPU_Prefetch(
	input wire i_reset,
	input wire i_clock,

	// Input
	input wire [31:0] i_address,
	output wire [31:0] o_address,
	output wire [31:0] o_rdata,

	// Bus
	output reg o_bus_request,
	input wire i_bus_ready,
	output reg [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata	
);

	reg [3:0] state;
	reg [31:0] address;
	reg [31:0] prefetch;

	assign o_address = address;
	assign o_rdata = prefetch;

	initial begin
		o_bus_request = 0;
		o_bus_address = 0;

		state = 0;
		address = 32'hffffffff;
	end

	always @(posedge i_clock) begin
		case (state)
			0: begin
				if (i_address != address) begin
					o_bus_request <= 1;
					o_bus_address <= i_address;
					state <= 1;
				end
			end

			1: begin
				if (i_bus_ready) begin
					o_bus_request <= 0;
					prefetch <= i_bus_rdata;
					address <= o_bus_address;
					state <= 0;
				end
			end
		endcase
	end

endmodule
