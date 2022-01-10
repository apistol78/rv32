
`timescale 1ns/1ns

module CPU_BusAccess(
	input wire i_reset,
	input wire i_clock,

	// Bus
	output reg o_bus_rw,				// Data read/write
	output wire o_bus_request,			// IO request.
	input wire i_bus_ready,				// IO request ready.
	output reg [31:0] o_bus_address,	// Address
	input wire [31:0] i_bus_rdata,		// Read data
	output reg [31:0] o_bus_wdata,		// Write data,

	// Port A
	input wire i_pa_rw,
	input wire i_pa_request,
	output wire o_pa_ready,
	input wire [31:0] i_pa_address,
	output wire [31:0] o_pa_rdata,
	input wire [31:0] i_pa_wdata,

	// Port B
	input wire i_pb_rw,
	input wire i_pb_request,
	output wire o_pb_ready,
	input wire [31:0] i_pb_address,
	output wire [31:0] o_pb_rdata,
	input wire [31:0] i_pb_wdata
);

	reg [1:0] state;

	initial begin
		state = 2'd0;
		o_bus_rw = 1'b0;
		o_bus_address = 0;
		o_bus_wdata = 0;
	end

	assign o_pa_ready = i_pa_request && (state == 2'd1) ? i_bus_ready : 1'b0;
	assign o_pb_ready = i_pb_request && (state == 2'd2) ? i_bus_ready : 1'b0;
	assign o_pa_rdata = i_pa_request && i_pa_rw == 1'b0 && (state == 2'd1) ? i_bus_rdata : 32'hz;
	assign o_pb_rdata = i_pb_request && i_pb_rw == 1'b0 && (state == 2'd2) ? i_bus_rdata : 32'hz;
	assign o_bus_request = (state != 2'd0) ? 1'b1 : 1'b0;

	always @(posedge i_clock) begin
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
						o_bus_rw <= i_pa_rw;
						o_bus_address <= i_pa_address;
						o_bus_wdata <= i_pa_wdata;
						state <= 2'd1;		
					end
				end

				// Wait until request has been processed.
				2'd1, 2'd2: begin
					if (i_bus_ready) begin
						state <= 2'd0;
					end
				end

				default:
					state <= 2'd0;

			endcase
		end
	end

endmodule
