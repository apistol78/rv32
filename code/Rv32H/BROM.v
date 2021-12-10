module BROM(
	input wire i_enable,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata
);

	reg [31:0] memory [0:699];
/*
   // Clear entire memory.
    integer i;
    initial begin
        $display("init ROM");
        for (i = 0; i < 700; i = i + 1)
            memory[i] = 32'h0201c783;
        memory[1] = 32'h0600006f;

    end
*/
    // Read ROM.
	initial $readmemh("code/Bare/Bare.vmem", memory);

	always @ (posedge i_enable) begin
		o_rdata <= memory[i_address >> 2];
	end

endmodule