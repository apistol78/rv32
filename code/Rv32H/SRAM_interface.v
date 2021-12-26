
// SRAM interface
module SRAM_interface(
	input wire i_clock,
	input wire i_request,
	input wire i_rw,
	input wire [17:0] i_address,
	input wire [15:0] i_wdata,
	output reg [15:0] o_rdata,
	output reg o_ready,

	output reg [17:0] SRAM_A,
	inout wire [15:0] SRAM_D,
	output reg SRAM_CE_n,
	output reg SRAM_OE_n,
	output reg SRAM_WE_n,
	output reg SRAM_LB_n,
	output reg SRAM_UB_n
);

	reg [6:0] state = 0;
	
	initial begin
		SRAM_CE_n <= 0;
		SRAM_OE_n <= 1;
		SRAM_WE_n <= 1;
		SRAM_LB_n <= 0;
		SRAM_UB_n <= 0;
		state <= 0;
		o_ready = 0;
	end
	
	assign SRAM_D = (i_request && i_rw) ? i_wdata : 16'hz;

	always @ (posedge i_clock) begin
		if (i_request) begin
			if (!i_rw) begin	// read
				case (state)
					0: begin
						SRAM_A <= i_address;
						SRAM_OE_n <= 0;						
						state <= 1;
					end

					1: begin
						o_rdata <= SRAM_D;
						state <= 2;
					end
			
					2: begin
						SRAM_OE_n <= 1;
						o_ready <= 1;
					end
				endcase
			end
			else begin	// write
				case (state)
					0: begin
						SRAM_A <= i_address;
						SRAM_WE_n <= 0;
						state <= 1;
					end

					1: begin
						SRAM_WE_n <= 1;		// Memory is stored when WE_n goes high.
						state <= 2;
					end

					2: begin
						o_ready <= 1;
					end
				endcase
			end
		end
		else begin
			state <= 0;
			SRAM_OE_n <= 1;
			SRAM_WE_n <= 1;
			o_ready <= 0;
		end
	end
endmodule