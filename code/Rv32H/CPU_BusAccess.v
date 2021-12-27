module CPU_BusAccess(
	input wire i_reset,
	input wire i_clock,

	// Bus
	output reg o_bus_rw,				// Data read/write
	output reg o_bus_request,			// IO request.
	input wire i_bus_ready,				// IO request ready.
	output reg [31:0] o_bus_address,	// Address
	input wire [31:0] i_bus_rdata,		// Read data
	output reg [31:0] o_bus_wdata,		// Write data,

	// Port A
	input wire i_pa_rw,
	input wire i_pa_request,
	output reg o_pa_ready,
	input wire [31:0] i_pa_address,
	output reg [31:0] o_pa_rdata,
	input wire [31:0] i_pa_wdata,

	// Port B
	input wire i_pb_rw,
	input wire i_pb_request,
	output reg o_pb_ready,
	input wire [31:0] i_pb_address,
	output reg [31:0] o_pb_rdata,
	input wire [31:0] i_pb_wdata
);

	reg [4:0] state = 0;

	initial begin
		o_bus_rw <= 0;
		o_bus_request <= 0;
		o_bus_address <= 0;
		o_bus_wdata <= 0;

		o_pa_ready <= 0;
		o_pa_rdata <= 0;

		o_pb_ready <= 0;
		o_pb_rdata <= 0;
	end

	always @(posedge i_clock) begin
		case (state)
			// Wait for request on any port.
			0: begin
				if (i_pa_request) begin
					$display("pa request %x begin", i_pa_address);
					o_bus_rw <= i_pa_rw;
					o_bus_request <= 1;
					o_bus_address <= i_pa_address;
					o_bus_wdata <= i_pa_wdata;
					o_pa_ready <= 0;
					state <= 1;
				end
				else if (i_pb_request) begin
					$display("pb request %x begin", i_pb_address);
					o_bus_rw <= i_pb_rw;
					o_bus_request <= 1;
					o_bus_address <= i_pb_address;
					o_bus_wdata <= i_pb_wdata;
					o_pb_ready <= 0;
					state <= 2;				
				end
				if (!i_pa_request) begin
					o_pa_ready <= 0;
				end
				if (!i_pb_request) begin
					o_pb_ready <= 0;
				end
			end

			// Process request from port A.
			1: begin
				if (i_bus_ready) begin
					$display("pa request finished");
					o_pa_ready <= 1;
					o_pa_rdata <= i_bus_rdata;
					o_bus_request <= 0;
					state <= 0;
				end
			end

			// Process request from port B.
			2: begin
				if (i_bus_ready) begin
					$display("pb request finished");
					o_pb_ready <= 1;
					o_pb_rdata <= i_bus_rdata;
					o_bus_request <= 0;
					state <= 0;
				end
			end
		endcase
	end

endmodule
