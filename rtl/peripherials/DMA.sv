
`timescale 1ns/1ns

module DMA(
	input i_reset,
	input i_clock,

	input i_request,
	input i_rw,
	input [1:0] i_address,
	input [31:0] i_wdata,
	output logic [31:0] o_rdata,
	output logic o_ready,

	// System
	input i_stall,

	// Bus
	output logic o_bus_rw,				// Data read/write
	output logic o_bus_request,			// IO request.
	input i_bus_ready,					// IO request ready.
	output logic [31:0] o_bus_address,	// Address
	input [31:0] i_bus_rdata,			// Read data
	output logic [31:0] o_bus_wdata		// Write data,

);
	typedef enum bit [1:0]
	{
		READ_REQ,
		WAIT_READ,
		WRITE_REQ,
		WAIT_WRITE
	} state_t;

	logic [31:0] from;
	logic [31:0] to;
	logic [31:0] count;
	logic [31:0] data;
	logic run = 0;
	state_t state = READ_REQ;

	initial begin
		o_ready = 0;
		o_bus_rw = 0;
		o_bus_request = 0;
		o_bus_address = 0;
		o_bus_wdata = 0;
	end

	always_ff @(posedge i_clock) begin

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
				READ_REQ: begin
					if (!i_stall) begin
						o_bus_request <= 1;
						o_bus_rw <= 0;
						o_bus_address <= from;
						state <= WAIT_READ;
					end
				end

				WAIT_READ: begin
					if (i_bus_ready) begin
						o_bus_request <= 0;
						data <= i_bus_rdata;
						from <= from + 4;
						state <= WRITE_REQ;
					end
				end

				WRITE_REQ: begin
					if (!i_stall) begin
						o_bus_request <= 1;
						o_bus_rw <= 1;
						o_bus_address <= to;
						o_bus_wdata <= data;
						state <= WAIT_WRITE;
					end
				end

				WAIT_WRITE: begin
					if (i_bus_ready) begin
						o_bus_request <= 0;
						to <= to + 4;
						if (count > 0) begin
							count <= count - 1;
						end
						else begin
							run <= 0;
						end
						state <= READ_REQ;
					end
				end		
			endcase
		end

	end

endmodule
