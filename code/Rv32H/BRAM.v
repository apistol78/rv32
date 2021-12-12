module BRAM(
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata
);

	reg [31:0] memory [0:32'hffff];

    // Clear entire memory.
    integer i;
    initial begin
        for (i = 0; i <= 32'hffff; i = i + 1)
            memory[i] = 0;
    end

	always @ (posedge i_enable) begin
        if (!i_rw) begin
		    o_rdata <= memory[i_address >> 2];
        end
        else begin
            memory[i_address >> 2] <= i_wdata;
        end
	end

endmodule