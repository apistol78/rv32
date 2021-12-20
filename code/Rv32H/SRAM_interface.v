
// SRAM interface
module SRAM_interface(
	input wire i_clock,
	input wire i_enable,
	input wire i_rw,
	input wire [17:0] i_address,
	input wire [15:0] i_wdata,
	output reg [15:0] o_rdata,
	output wire o_ready,

	output reg [17:0] SRAM_A,
	inout wire [15:0] SRAM_D,
	output reg SRAM_CE_n,
	output reg SRAM_OE_n,
	output reg SRAM_WE_n,
	output reg SRAM_LB_n,
	output reg SRAM_UB_n
);

	localparam STATE_END = 30;

	reg [6:0] state = 0;
	
	assign SRAM_D = (i_enable && i_rw) ? i_wdata : 16'hzzzz;
	assign o_ready = (i_enable && state == STATE_END);
	
	initial begin
		SRAM_CE_n <= 1;
		SRAM_OE_n <= 1;
		SRAM_WE_n <= 1;
		SRAM_LB_n <= 0;
		SRAM_UB_n <= 0;
	end

	always @ (posedge i_clock) begin
		if (i_enable) begin
			if (!i_rw) begin	// read
				case (state)
					0: begin
						SRAM_A <= i_address;
						state <= 5;
					end
					
					1, 2, 3, 4: begin
						state <= state + 1;
					end

					5: begin
						SRAM_CE_n <= 0;
						SRAM_OE_n <= 0;
						state <= 10;
					end
					
					6, 7, 8, 9: begin
						state <= state + 1;
					end
				
					10: begin
						o_rdata <= SRAM_D;
						state <= STATE_END;
					end
				endcase
			end
			else begin	// write
				case (state)
					0: begin
						SRAM_A <= i_address;
						state <= 5;
					end
					
					1, 2, 3, 4: begin
						state <= state + 1;
					end

					5: begin
						SRAM_CE_n <= 0;
						SRAM_WE_n <= 0;
						state <= 10;
					end
					
					6, 7, 8, 9: begin
						state <= state + 1;
					end

					10: begin
						SRAM_CE_n <= 1;
						SRAM_WE_n <= 1;
						state <= 15;
					end
						
					11, 12, 13, 14: begin
						state <= state + 1;
					end
					
					15: begin
						state <= STATE_END;
					end
				endcase
			end
		end
		else begin
			state <= 0;
			SRAM_CE_n <= 1;
			SRAM_OE_n <= 1;
			SRAM_WE_n <= 1;			
		end
	end

endmodule