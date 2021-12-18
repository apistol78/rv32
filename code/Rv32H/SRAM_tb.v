
// Fake SRAM module, for testing wait signal.
module SRAM_tb(
	input wire i_clock,
	input wire i_enable,
	input wire i_rw,
	input wire [17:0] i_address,
	input wire [15:0] i_wdata,
	output reg [15:0] o_rdata,
	output reg o_ready
);

	reg [15:0] data [0:32'h0040_0000 - 1];
	reg [17:0] address;
	reg ce_n;
	reg oe_n;
	reg we_n;
	reg [4:0] state = 0;

	initial begin
		o_ready <= 0;
		state <= 0;
		ce_n <= 1;
		oe_n <= 1;
		we_n <= 1;
	end

	always @ (posedge i_clock) begin
		if (i_enable) begin
			state <= state + 1;
		end
		else begin
			o_ready <= 0;
			state <= 0;
			ce_n <= 1;
			oe_n <= 1;
			we_n <= 1;			
		end
	end

	always @ (posedge i_clock) begin
		if (i_enable && !i_rw) begin
			case (state)
				0: begin
					address <= i_address;
				end
					
				1: begin
					ce_n <= 0;
					oe_n <= 0;
				end

				2: begin
					$display("mem read ready %x, value %x", address, data[address]);
					o_rdata <= data[address];
					o_ready <= 1;
				end
			endcase
		end
		else if (i_enable && i_rw) begin
			case (state)
				0: begin
					address <= i_address;
				end

				1: begin
					ce_n <= 0;
					we_n <= 0;
				end

				2: begin
					$display("mem write ready %x, value %x", address, i_wdata);
					data[address] <= i_wdata;
					o_ready <= 1;
				end
			endcase
		end
	end

endmodule