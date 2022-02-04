
`timescale 1ns/1ns

module DMA(
	input wire i_reset,
	input wire i_clock,

	input wire i_request,
	input wire i_rw,
	input wire [1:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output reg o_ready,

	// System
	input wire i_stall,

	// Bus
	output reg o_bus_rw,				// Data read/write
	output reg o_bus_request,			// IO request.
	input wire i_bus_ready,				// IO request ready.
	output reg [31:0] o_bus_address,	// Address
	input wire [31:0] i_bus_rdata,		// Read data
	output reg [31:0] o_bus_wdata		// Write data,

);

	reg [31:0] from;
	reg [31:0] to;
	reg [31:0] count;
	reg [31:0] data;
	reg run;
	reg [2:0] state;

	initial begin
		run = 0;
		state = 0;

		o_ready = 0;
		o_bus_rw = 0;
		o_bus_request = 0;
		o_bus_address = 0;
		o_bus_wdata = 0;
	end

	always @(posedge i_clock) begin

		o_ready <= 0;

		if (i_request) begin

			if (!i_rw) begin
				if (i_address == 2'b11) begin
					o_rdata <= run ? 32'hffff_ffff : 32'h0000_0000;
					o_ready <= 1;
				end
			end
			else begin
				// Receive commands from CPU.
				if (i_address == 2'b00) begin
					from <= i_wdata;
					o_ready <= 1;
				end
				else if (i_address == 2'b01) begin
					to <= i_wdata;
					o_ready <= 1;
				end
				else if (i_address == 2'b10) begin
					count <= i_wdata;
					o_ready <= 1;
				end
				else if (i_address == 2'b11) begin
					run <= 1;
					o_ready <= 1;
				end
			end

		end

		// Execute transfer.
		if (run) begin
			case (state)
				0: begin
					if (!i_stall) begin
						o_bus_request <= 1;
						o_bus_rw <= 0;
						o_bus_address <= from;
						state <= 1;
					end
				end

				1: begin
					if (i_bus_ready) begin
						o_bus_request <= 0;
						data <= i_bus_rdata;
						from <= from + 4;
						state <= 2;
					end
				end

				2: begin
					if (!i_stall) begin
						o_bus_request <= 1;
						o_bus_rw <= 1;
						o_bus_address <= to;
						o_bus_wdata <= data;
						state <= 3;
					end
				end

				3: begin
					if (i_bus_ready) begin
						o_bus_request <= 0;
						to <= to + 4;
						if (count > 0) begin
							count <= count - 1;
							state <= 0;
						end
						else begin
							run <= 0;
							state <= 0;
						end
					end
				end		
			endcase
		end

	end

endmodule
