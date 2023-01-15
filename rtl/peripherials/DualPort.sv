
`timescale 1ns/1ns

module DualPort(
	input i_reset,
	input i_clock,

	// Bus
	output bit o_bus_rw,				// Data read/write
	output bit o_bus_request,			// IO request.
	input i_bus_ready,					// IO request ready.
	output bit [31:0] o_bus_address,	// Address
	input [31:0] i_bus_rdata,			// Read data
	output bit [31:0] o_bus_wdata,		// Write data,

	// Port B
	input i_pb_rw,
	input i_pb_request,
	output bit o_pb_ready,
	input [31:0] i_pb_address,
	output bit [31:0] o_pb_rdata,
	input [31:0] i_pb_wdata,

	// Port C
	input i_pc_rw,
	input i_pc_request,
	output bit o_pc_ready,
	input [31:0] i_pc_address,
	output bit [31:0] o_pc_rdata,
	input [31:0] i_pc_wdata
);

	bit [1:0] state = 0;

	initial begin
		o_bus_rw = 1'b0;
		o_bus_request = 1'b0;
		o_bus_address = 0;
		o_bus_wdata = 0;
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			state <= 2'd0;

			o_bus_rw <= 1'b0;
			o_bus_request <= 1'b0;
			o_bus_address <= 0;
			o_bus_wdata <= 0;
		end
		else begin

			case (state)

				// Wait for any request.
				2'd0: begin
					o_bus_request <= 1'b0;

					o_pb_ready <= 1'b0;
					o_pc_ready <= 1'b0;

					if (i_pb_request) begin
						o_bus_rw <= i_pb_rw;
						o_bus_request <= 1'b1;
						o_bus_address <= i_pb_address;
						o_bus_wdata <= i_pb_wdata;
						state <= 2'd1;
					end
					else if (i_pc_request) begin
						o_bus_rw <= i_pc_rw;
						o_bus_request <= 1'b1;
						o_bus_address <= i_pc_address;
						o_bus_wdata <= i_pc_wdata;
						state <= 2'd2;
					end					
				end

				// Wait until request has been processed.
				2'd1: begin
					if (i_bus_ready) begin
						o_bus_request <= 1'b0;
						o_pb_rdata <= i_bus_rdata;
						o_pb_ready <= 1'b1;
						state <= 2'd3;
					end
				end

				2'd2: begin
					if (i_bus_ready) begin
						o_bus_request <= 1'b0;
						o_pc_rdata <= i_bus_rdata;
						o_pc_ready <= 1'b1;
						state <= 2'd3;
					end
				end

				2'd3: begin
					o_pb_ready <= 1'b0;
					o_pc_ready <= 1'b0;
					if (!i_bus_ready)
						state <= 2'd0;
				end

				default:
					state <= 2'd0;

			endcase
		end
	end

endmodule
