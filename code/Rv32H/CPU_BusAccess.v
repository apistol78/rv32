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

	initial begin
		o_bus_rw <= 0;
		o_bus_request <= 0;
		o_bus_address <= 0;
		o_bus_wdata <= 0;

		o_pa_ready <= 0;
		o_pb_ready <= 0;
	end

	always @(*) begin

		if (i_pb_request) begin
			o_bus_rw <= i_pb_rw;
			o_bus_request <= 1;
			o_bus_address <= i_pb_address;
			o_bus_wdata <= i_pb_wdata;
			o_pb_ready <= i_bus_ready;
			o_pb_rdata <= i_bus_rdata;

			o_pa_ready <= 0;			
		end
		else if (i_pa_request) begin
			o_bus_rw <= i_pa_rw;
			o_bus_request <= 1;
			o_bus_address <= i_pa_address;
			o_bus_wdata <= i_pa_wdata;
			o_pa_ready <= i_bus_ready;
			o_pa_rdata <= i_bus_rdata;

			o_pb_ready <= 0;
		end
		else begin
			o_bus_request <= 0;
		end
	end

endmodule
