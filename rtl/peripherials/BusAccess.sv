
`timescale 1ns/1ns

module BusAccess #(
	parameter REGISTERED
)(
	input i_reset,
	input i_clock,

	// Bus
	output bit o_bus_rw,				// Data read/write
	output o_bus_request,				// IO request.
	input i_bus_ready,					// IO request ready.
	output bit [31:0] o_bus_address,	// Address
	input [31:0] i_bus_rdata,			// Read data
	output bit [31:0] o_bus_wdata,		// Write data,

	// Port A (Read only)
	input i_pa_request,
	output o_pa_ready,
	input [31:0] i_pa_address,
	output [31:0] o_pa_rdata,
	output o_pa_busy,

	// Port B
	input i_pb_rw,
	input i_pb_request,
	output o_pb_ready,
	input [31:0] i_pb_address,
	output [31:0] o_pb_rdata,
	input [31:0] i_pb_wdata,
	output o_pb_busy,

	// Port C
	input i_pc_rw,
	input i_pc_request,
	output o_pc_ready,
	input [31:0] i_pc_address,
	output [31:0] o_pc_rdata,
	input [31:0] i_pc_wdata,
	output o_pc_busy
);

	bit [1:0] state = 0;
	bit [1:0] next_state = 0;

	initial begin
		o_bus_rw = 1'b0;
		o_bus_address = 0;
		o_bus_wdata = 0;
	end

	generate if (!REGISTERED) begin

		assign o_pa_ready = i_pa_request && (state == 2'd1) ? i_bus_ready : 1'b0;
		assign o_pb_ready = i_pb_request && (state == 2'd2) ? i_bus_ready : 1'b0;
		assign o_pc_ready = i_pc_request && (state == 2'd3) ? i_bus_ready : 1'b0;

		assign o_pa_rdata = i_bus_rdata;
		assign o_pb_rdata = i_bus_rdata;
		assign o_pc_rdata = i_bus_rdata;

		assign o_pa_busy = (state == 2'd1);
		assign o_pb_busy = (state == 2'd2);
		assign o_pc_busy = (state == 2'd3);

		assign o_bus_request = (state != 2'd0) & (i_pa_request | i_pb_request | i_pc_request);

		always_ff @(posedge i_clock) begin
			state <= next_state;
		end

		always_comb begin

			next_state = state;

			o_bus_rw = 0;
			o_bus_address = 0;
			o_bus_wdata = 0;
			
			case (state)

				// Wait for any request.
				2'd0: begin
					if (i_pb_request) begin
						o_bus_rw = i_pb_rw;
						o_bus_address = i_pb_address;
						o_bus_wdata = i_pb_wdata;
						next_state = 2'd2;
					end
					else if (i_pa_request) begin
						o_bus_rw = 1'b0;
						o_bus_address = i_pa_address;
						next_state = 2'd1;		
					end
					else if (i_pc_request) begin
						o_bus_rw = i_pc_rw;
						o_bus_address = i_pc_address;
						o_bus_wdata = i_pc_wdata;
						next_state = 2'd3;
					end					
				end

				// Wait until request has been processed.
				2'd1: begin
					o_bus_address = i_pa_address;
					if (i_bus_ready) begin
						next_state = 2'd0;
					end
				end
				2'd2: begin
					o_bus_rw = i_pb_rw;
					o_bus_address = i_pb_address;
					o_bus_wdata = i_pb_wdata;
					if (i_bus_ready) begin
						next_state = 2'd0;
					end
				end
				2'd3: begin
					o_bus_rw = i_pc_rw;
					o_bus_address = i_pc_address;
					o_bus_wdata = i_pc_wdata;
					if (i_bus_ready) begin
						next_state = 2'd0;
					end
				end

				default:
					next_state = 2'd0;

			endcase
		end
	end endgenerate

	generate if (REGISTERED) begin

		assign o_pa_ready = i_pa_request && (state == 2'd1) ? i_bus_ready : 1'b0;
		assign o_pb_ready = i_pb_request && (state == 2'd2) ? i_bus_ready : 1'b0;
		assign o_pc_ready = i_pc_request && (state == 2'd3) ? i_bus_ready : 1'b0;

		assign o_pa_rdata = i_bus_rdata;
		assign o_pb_rdata = i_bus_rdata;
		assign o_pc_rdata = i_bus_rdata;

		assign o_pa_busy = (state == 2'd1);
		assign o_pb_busy = (state == 2'd2);
		assign o_pc_busy = (state == 2'd3);

		assign o_bus_request = (state != 2'd0) ? 1'b1 : 1'b0;

		always_ff @(posedge i_clock) begin
			if (i_reset) begin
				state <= 2'd0;
				o_bus_rw <= 1'b0;
				o_bus_address <= 0;
				o_bus_wdata <= 0;
			end
			else begin
				case (state)

					// Wait for any request.
					2'd0: begin
						if (i_pb_request) begin
							o_bus_rw <= i_pb_rw;
							o_bus_address <= i_pb_address;
							o_bus_wdata <= i_pb_wdata;
							state <= 2'd2;
						end
						else if (i_pa_request) begin
							o_bus_rw <= 1'b0;
							o_bus_address <= i_pa_address;
							state <= 2'd1;		
						end
						else if (i_pc_request) begin
							o_bus_rw <= i_pc_rw;
							o_bus_address <= i_pc_address;
							o_bus_wdata <= i_pc_wdata;
							state <= 2'd3;
						end					
					end

					// Wait until request has been processed.
					2'd1, 2'd2, 2'd3: begin
						if (i_bus_ready) begin
							state <= 2'd0;
						end
					end

					default:
						state <= 2'd0;

				endcase
			end
		end

	end endgenerate

endmodule
