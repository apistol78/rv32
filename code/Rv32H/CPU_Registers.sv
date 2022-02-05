`include "CPU_Types.sv"

`timescale 1ns/1ns

module CPU_Registers (
	input i_reset,
	input i_clock,

	input fetch_data_t i_fetch_data,
	output [31:0] o_rs1,
	output [31:0] o_rs2,

	input memory_data_t i_memory_data
);
	assign o_rs1 = rs1;
	assign o_rs2 = rs2;

	reg [`TAG_SIZE] read_tag = 0;
	reg [`TAG_SIZE] write_tag = 0;

	reg [31:0] r[31:0];
	reg [31:0] rs1 = 0;
	reg [31:0] rs2 = 0;

	wire [4:0] inst_rs1 = i_fetch_data.instruction[19:15];
	wire [4:0] inst_rs2 = i_fetch_data.instruction[24:20];

    initial begin
        r[ 0] = 32'h0000_0000;
        r[ 1] = 32'h0000_0000;
        r[ 2] = 32'h1000_0400 - 4;	// sp	
        r[ 3] = 32'h0000_0000;
        r[ 4] = 32'h0000_0000;
        r[ 5] = 32'h0000_0000;
        r[ 6] = 32'h0000_0000;
        r[ 7] = 32'h0000_0000;
        r[ 8] = 32'h0000_0000;
        r[ 9] = 32'h0000_0000;
        r[10] = 32'h0000_0000;
        r[11] = 32'h0000_0000;
        r[12] = 32'h0000_0000;
        r[13] = 32'h0000_0000;
        r[14] = 32'h0000_0000;
        r[15] = 32'h0000_0000;
        r[16] = 32'h0000_0000;
        r[17] = 32'h0000_0000;
        r[18] = 32'h0000_0000;
        r[19] = 32'h0000_0000;
        r[20] = 32'h0000_0000;
        r[21] = 32'h0000_0000;
        r[22] = 32'h0000_0000;
        r[23] = 32'h0000_0000;
        r[24] = 32'h0000_0000;
        r[25] = 32'h0000_0000;
        r[26] = 32'h0000_0000;
        r[27] = 32'h0000_0000;
        r[28] = 32'h0000_0000;
        r[29] = 32'h0000_0000;
        r[30] = 32'h0000_0000;
        r[31] = 32'h0000_0000;       
    end

	always @(posedge i_clock, posedge i_reset)
	begin
		if (i_reset) begin
			rs1 <= 0;
			rs2 <= 0;

			read_tag <= 0;
			write_tag <= 0;

			r[ 0] <= 32'h0000_0000;
			r[ 1] <= 32'h0000_0000;
			r[ 2] <= 32'h1000_0400 - 4;	// sp	
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
			if (i_fetch_data.tag != read_tag) begin
                rs1 <= (inst_rs1 != 0) ? r[inst_rs1] : 32'h0;
                rs2 <= (inst_rs2 != 0) ? r[inst_rs2] : 32'h0;
				read_tag <= i_fetch_data.tag;
			end
			if (i_memory_data.tag != write_tag) begin
				r[i_memory_data.inst_rd] <= i_memory_data.rd;
				write_tag <= i_memory_data.tag;
			end
		end
	end

endmodule