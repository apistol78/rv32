module Registers(
	input wire i_reset,
	input wire i_clock,

	input wire [4:0] i_rs1_idx,
	input wire [4:0] i_rs2_idx,
	input wire [4:0] i_rd_idx,

	input wire i_need_rs1,
	input wire i_need_rs2,
	input wire i_need_rd,

	output reg [31:0] o_rs1,
	output reg [31:0] o_rs2,
	input wire [31:0] i_rd,

	input wire i_wr_request
);

	reg [31:0] r[31:0];

	/*
	integer i;
	initial begin
		for (i = 0; i < 32; i = i + 1)
			r[i] <= 32'h0000_0000;
		r[2] <= 32'h0001_0400;	// sp
	end
	*/

	always @(*) begin
		o_rs1 = i_need_rs1 && (i_rs1_idx != 0) ? r[i_rs1_idx] : 32'h0;
		o_rs2 = i_need_rs2 && (i_rs2_idx != 0) ? r[i_rs2_idx] : 32'h0;	
	end

	always @(posedge i_clock, posedge i_reset)
	begin
		if (i_reset) begin
			
			r[ 0] <= 32'h0000_0000;
			r[ 1] <= 32'h0000_0000;
			r[ 2] <= 32'h0001_0400;	// sp	
			r[ 3] <= 32'h0000_0000;
			r[ 4] <= 32'h0000_0000;
			r[ 5] <= 32'h0000_0000;
			r[ 6] <= 32'h0000_0000;
			r[ 7] <= 32'h0000_0000;
			r[ 8] <= 32'h0000_0000;
			r[ 9] <= 32'h0000_0000;
			r[10] <= 32'h0000_0000;
			r[11] <= 32'h0000_0000;
			r[12] <= 32'h0000_0000;
			r[13] <= 32'h0000_0000;
			r[14] <= 32'h0000_0000;
			r[15] <= 32'h0000_0000;
			r[16] <= 32'h0000_0000;
			r[17] <= 32'h0000_0000;
			r[18] <= 32'h0000_0000;
			r[19] <= 32'h0000_0000;
			r[20] <= 32'h0000_0000;
			r[21] <= 32'h0000_0000;
			r[22] <= 32'h0000_0000;
			r[23] <= 32'h0000_0000;
			r[24] <= 32'h0000_0000;
			r[25] <= 32'h0000_0000;
			r[26] <= 32'h0000_0000;
			r[27] <= 32'h0000_0000;
			r[28] <= 32'h0000_0000;
			r[29] <= 32'h0000_0000;
			r[30] <= 32'h0000_0000;
			r[31] <= 32'h0000_0000;
		
		end
		else begin
			if (i_wr_request && i_need_rd) begin
				$display("WRITE REGISTER rd[%d] = %x", i_rd_idx, i_rd);
				r[i_rd_idx] <= i_rd;
			end
		end
	end

endmodule