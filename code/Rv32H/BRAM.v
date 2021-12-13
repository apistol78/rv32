module BRAM(
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata
);

	reg [31:0] memory [32'hffff:0];

    // Clear entire memory.
    integer i;
    initial begin
        for (i = 0; i <= 32'hffff; i = i + 1)
            memory[i] = 0;
    end

	always @ (posedge i_enable) begin
        if (!i_rw) begin
            $display("-- read mem %x (%x) --", i_address, memory[i_address >> 2]);
		    o_rdata <= memory[i_address >> 2];
        end
        else begin
            $display("-- write mem %x (%x) --", i_address, i_wdata);
            memory[i_address >> 2] <= i_wdata;
        end
	end

endmodule