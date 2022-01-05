`include "CPU_Defines.v"

module CPU_Memory(
	input wire i_reset,
	input wire i_clock,

	// Bus
	output reg o_bus_rw,
	output reg o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,
	output reg [31:0] o_bus_wdata,

	// Input
	input wire [`TAG_SIZE] i_tag,
	input wire [4:0] i_inst_rd,
	input wire [31:0] i_rd,
	input wire [31:0] i_pc_next,
	input wire i_mem_read,
	input wire i_mem_write,
	input wire [2:0] i_mem_width,
	input wire i_mem_signed,
	input wire [31:0] i_mem_address,

	// Output
	output reg [`TAG_SIZE] o_tag,
	output reg [4:0] o_inst_rd,
	output reg [31:0] o_rd,
	output reg [31:0] o_pc_next,
	output wire o_stall
);

	localparam STATE_RMW_READ			= 1;
	localparam STATE_RMW_RST_REQUEST	= 2;
	localparam STATE_RMW_WAIT_WRITE		= 3;

	reg [4:0] state;

	initial begin
		state <= 0;
		o_bus_rw <= 0;
		o_bus_request <= 0;
		o_bus_wdata <= 0;
		o_tag <= 0;
		o_inst_rd <= 0;
		o_rd <= 0;
		o_pc_next <= 0;
	end

	// Stall pipeline if we perform a memory access.
	assign o_stall = (i_tag != o_tag) && (i_mem_read || i_mem_write);

	assign o_bus_address = { i_mem_address[31:2], 2'b00 };
	// assign o_bus_rw =
	// 	i_mem_width == 4 ? i_mem_write :
	// 	i_mem_width == 2 ? (i_mem_write && (state == STATE_RMW_WAIT_WRITE)) :
	// 	i_mem_width == 1 ? (i_mem_write && (state == STATE_RMW_WAIT_WRITE)) :
	// 	1'b0;
/*
	assign o_bus_request = 
		(i_mem_read || i_mem_write) && (i_tag != o_tag) && (state != STATE_RMW_RST_REQUEST);
*/
	wire [1:0] address_byte_index = i_mem_address[1:0];
	wire [7:0] bus_rdata_byte = i_bus_rdata >> (address_byte_index * 8);
	wire [15:0] bus_rdata_half = i_bus_rdata >> (address_byte_index * 8);

	always @(posedge i_clock) begin
		if (i_reset) begin
			state <= 0;
			o_bus_rw <= 0;
			o_bus_request <= 0;
			o_bus_wdata <= 0;
			o_tag <= 0;
			o_inst_rd <= 0;
			o_rd <= 0;
			o_pc_next <= 0;
		end
		else begin
			case(state)
				0: begin
					if (i_tag != o_tag) begin
						if (i_mem_read) begin
							// Single read cycle.
							o_bus_request <= 1;
							if (i_bus_ready) begin
								case (i_mem_width)
									4: o_rd <= i_bus_rdata;
									2: o_rd <= { { 16{ i_mem_signed & bus_rdata_half[15] } }, bus_rdata_half[15:0] };
									1: o_rd <= { { 24{ i_mem_signed & bus_rdata_byte[ 7] } }, bus_rdata_byte[ 7:0] };
								endcase
								
								// Finish
								o_bus_request <= 0;
								o_pc_next <= i_pc_next; 
								o_inst_rd <= i_inst_rd;
								o_tag <= i_tag;
							end
						end
						else if (i_mem_write) begin
							o_bus_request <= 1;
							
							if (i_mem_width == 4) begin
								// Single write cycle.
								o_bus_rw <= 1;
								o_bus_wdata <= i_rd;

								if (i_bus_ready) begin
								
									// Finish
									o_bus_rw <= 0;
									o_bus_request <= 0;
									o_pc_next <= i_pc_next; 
									o_inst_rd <= i_inst_rd;
									o_tag <= i_tag;
								end
							end
							else begin
								// Read-modify-write cycle.
								o_bus_rw <= 0;
								state <= STATE_RMW_READ;
							end
						end
						else begin
							// Finish, no memory access.
							o_pc_next <= i_pc_next; 
							o_inst_rd <= i_inst_rd;
							o_rd <= i_rd;
							o_tag <= i_tag;
						end
					end
				end

				// =============
				// Read-modify-write
				STATE_RMW_READ: begin
					if (i_bus_ready) begin
						// Modify value.
						o_bus_request <= 0;
						o_bus_rw <= 1;
						if (i_mem_width == 1) begin
							case ( address_byte_index  )
								2'd0: o_bus_wdata <= { i_bus_rdata[31:24], i_bus_rdata[23:16], i_bus_rdata[15:8],        i_rd[7:0] };
								2'd1: o_bus_wdata <= { i_bus_rdata[31:24], i_bus_rdata[23:16],         i_rd[7:0], i_bus_rdata[7:0] };
								2'd2: o_bus_wdata <= { i_bus_rdata[31:24],          i_rd[7:0], i_bus_rdata[15:8], i_bus_rdata[7:0] };
								2'd3: o_bus_wdata <= {          i_rd[7:0], i_bus_rdata[23:16], i_bus_rdata[15:8], i_bus_rdata[7:0] };
							endcase
						end
						else begin	// width must be 2
							case ( address_byte_index  )
								2'd0: o_bus_wdata <= { i_bus_rdata[31:16],        i_rd[15:0] };
								2'd2: o_bus_wdata <= {         i_rd[15:0], i_bus_rdata[15:0] };
							endcase						
						end
						state <= STATE_RMW_RST_REQUEST;
					end
				end

				STATE_RMW_RST_REQUEST: begin
					o_bus_request <= 1;
					state <= STATE_RMW_WAIT_WRITE;
				end

				STATE_RMW_WAIT_WRITE: begin
					if (i_bus_ready) begin
						// Finish
						o_bus_request <= 0;
						o_bus_rw <= 0;
						o_pc_next <= i_pc_next; 
						o_inst_rd <= i_inst_rd;
						o_tag <= i_tag;
						state <= 0;
					end
				end
			endcase
		end
	end

endmodule
