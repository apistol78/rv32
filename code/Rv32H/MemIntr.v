module MemIntr(
	input wire i_clock,
	input wire i_enable,
	input wire i_rw,

	input wire [31:0] i_cpu_address,
	input wire [31:0] i_cpu_wdata,
	output reg [31:0] o_cpu_rdata,

	output wire [31:0] o_mem_address,
	input wire [31:0] i_mem_wdata,
	output reg [31:0] o_mem_rdata,
);

	wire [1:0] byte = i_addr[1:0];
	wire is_aligned = (byte == 2'b00);


	assign o_mem_address = i_cpu_address & 32'hffff_fffc;	// aligned address


	always @ (posedge i_enable) begin

		if (is_aligned) begin

			if (i_rw == 1'b0) begin // read
			end
			else begin	// write
				
			end

		end
		else begin

			$display("Unaligned memory access");

		end

	end

endmodule
