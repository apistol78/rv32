module CPU_Registers (
	input wire i_reset,
	input wire i_clock,

	input wire [7:0] i_read_tag,
	input wire [4:0] i_read_rs1_idx,
	input wire [4:0] i_read_rs2_idx,
	output reg [31:0] o_rs1,
	output reg [31:0] o_rs2,

	input wire [7:0] i_write_tag,
	input wire [4:0] i_write_rd_idx,
	input wire [31:0] i_rd
);
	reg [7:0] read_tag;
	reg [7:0] write_tag;

	reg [31:0] r[31:0];

    initial begin
		o_rs1 <= 0;
		o_rs2 <= 0;

		read_tag <= 0;
		write_tag <= 0;

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
			if (i_read_tag != read_tag) begin
                o_rs1 <= (i_read_rs1_idx != 0) ? r[i_read_rs1_idx] : 32'h0;
                o_rs2 <= (i_read_rs1_idx != 0) ? r[i_read_rs2_idx] : 32'h0;
				read_tag <= i_read_tag;
			end
			if (i_write_tag != write_tag) begin
				r[i_write_rd_idx] <= i_rd;
				write_tag <= i_write_tag;
			end
		end
	end

endmodule