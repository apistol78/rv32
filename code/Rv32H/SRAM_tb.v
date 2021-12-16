// Fake SRAM module, for testing wait signal.
module SRAM_tb(
    input wire i_clock,
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
    output reg o_ready
);

	reg [31:0] data [0:32'h1fff];

    reg [4:0] tick;
    reg [31:0] addr;
    reg [31:0] wdata;

	integer i;
	initial begin
		for (i = 0; i < 32'h1000; i = i + 1) begin
			data[i] = 0;
			data[i + 32'h1000] = 0;
		end
        o_ready = 0;
	end

	always @ (posedge i_enable) begin
        o_ready <= 0;
		if (!i_rw) begin
            o_rdata <= 32'hxxxx_xxxx;
            addr <= i_address;
            tick <= 4;
		end
		else begin
            wdata <= i_wdata;
            addr <= i_address;
            tick <= 4;
		end
	end

    always @ (posedge i_clock) begin
        if (i_enable) begin
            tick <= tick - 1;
            if (tick == 0) begin
                if (!i_rw) begin
                    $display("mem read ready");
                    o_rdata <= data[addr >> 2];
                    o_ready <= 1;
                end
                else begin
                    $display("mem write ready");
                    data[addr >> 2] <= wdata;
                    o_ready <= 1;
                end
            end
        end
        else begin
            o_ready <= 0;
        end
    end

endmodule