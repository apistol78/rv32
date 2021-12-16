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

	reg [31:0] data [0:32'h0040_0000 - 1];

    reg [4:0] tick;
    reg [31:0] addr;
    reg [31:0] wdata;

	initial begin
		o_ready = 0;
		tick <= 4;
	end

    always @ (posedge i_clock) begin
        if (i_enable) begin
		  
				if (tick == 4) begin
					if (!i_rw) begin
							o_rdata <= 32'hxxxx_xxxx;
							addr <= i_address;
					end
					else begin
							wdata <= i_wdata;
							addr <= i_address;
					end
					tick <= tick - 1;
				end
				else if (tick == 0) begin
                o_ready <= 1;
                if (!i_rw) begin
                    $display("mem read ready");
                    o_rdata <= data[addr >> 2];
                end
                else begin
                    $display("mem write ready");
                    data[addr >> 2] <= wdata;
                end
            end
				else begin
					tick <= tick - 1;
				end
				
        end
        else begin
            o_ready <= 0;
				tick <= 4;
        end
    end

endmodule