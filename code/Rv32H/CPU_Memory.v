module CPU_Memory(
	input wire i_reset,
	input wire i_clock,
	input wire i_stall,

	// Bus
	output wire o_bus_rw,
	output wire o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,
	output reg [31:0] o_bus_wdata,

	// Input
	input wire [7:0] i_tag,
	input wire [4:0] i_inst_rd,
	input wire [31:0] i_rd,
	input wire i_branch,
	input wire [31:0] i_pc_next,
	input wire i_mem_read,
	input wire i_mem_write,
	input wire [2:0] i_mem_width,
	input wire i_mem_signed,
	input wire [31:0] i_mem_address,

	// Output
	output reg [7:0] o_tag,
	output reg [4:0] o_inst_rd,
	output reg [31:0] o_rd,
	output reg o_branch,
	output reg [31:0] o_pc_next,
	output wire o_stall
);

	initial begin
		o_bus_wdata <= 0;
		o_tag <= 0;
		o_inst_rd <= 0;
		o_rd <= 0;
		o_branch <= 0;
		o_pc_next <= 0;
	end

	assign o_bus_rw = i_mem_write;
	assign o_bus_address = i_mem_address;
	assign o_bus_request = (i_mem_read || i_mem_write) && (i_tag != o_tag);
	assign o_stall = (i_tag != o_tag) && (i_mem_read || i_mem_write);

	always @(posedge i_clock) begin
		if (i_tag != o_tag) begin
			o_inst_rd <= i_inst_rd;
			
			if (i_mem_read) begin
				// Extend the data received from bus.
				case (i_mem_width)
					4: o_rd <= i_bus_rdata;
					2: o_rd <= { { 16{ i_mem_signed & i_bus_rdata[15] } }, i_bus_rdata[15:0] };
					1: o_rd <= { { 24{ i_mem_signed & i_bus_rdata[7] } }, i_bus_rdata[7:0] };
				endcase
			end
			else
				o_rd <= i_rd;

			o_bus_wdata <= i_rd;

			o_branch <= i_branch;
			o_pc_next <= i_pc_next;    

			if (i_mem_read || i_mem_write) begin
				if (i_bus_ready)
					o_tag <= i_tag;
			end
			else begin
				o_tag <= i_tag;
			end
		end
	end

endmodule
