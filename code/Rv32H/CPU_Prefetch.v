
`timescale 1ns/1ns

module CPU_Prefetch(
	input wire i_reset,
	input wire i_clock,

	// Input
	input wire i_request,
	output reg o_ready,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata,

	// Bus
	output reg o_bus_request,
	input wire i_bus_ready,
	output reg [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata	
);

	reg [3:0] state;
	reg [31:0] address;
	reg [31:0] prefetch;

	initial begin
		o_ready = 0;
		o_rdata = 0;
		o_bus_request = 0;
		o_bus_address = 0;

		state = 0;
		address = 32'hffffffff;
	end

	always @(posedge i_clock) begin
		case (state)
			0: begin
				if (i_request) begin
					if (i_address == address) begin
						// Prefetched word is valid, return word to CPU but also
						// simultaneously prefetch next word.
						o_rdata <= prefetch;
						o_ready <= 1;
						o_bus_request <= 1;
						o_bus_address <= i_address + 4;
						state <= 1;
					end
					else begin
						// Prefetched word not valid, request word from bus.
						o_bus_request <= 1;
						o_bus_address <= i_address;
						state <= 2;
					end
				end
				else
					o_ready <= 0;
			end

			1: begin
				// Wait until bus read finished, fetched word is
				// stored as prefetched word for next CPU request.
				o_ready <= 0;
				if (i_bus_ready) begin
					o_bus_request <= 0;
					prefetch <= i_bus_rdata;
					address <= o_bus_address;
					state <= 0;
				end
			end

			2: begin
				// Wait until bus read finished, signal CPU word is ready but
				// also continue prefetching next word.
				if (i_bus_ready) begin
					o_rdata <= i_bus_rdata;
					o_ready <= 1;
					o_bus_address <= o_bus_address + 4;
					state <= 1;
				end
			end
		endcase
	end

endmodule
