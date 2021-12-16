
// SRAM interface
module SRAM_interface(
    input wire i_clock,
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
    output reg o_ready,

	output reg [17:0] SRAM_A,
	inout wire [15:0] SRAM_D,
	output reg SRAM_CE_n,
	output reg SRAM_OE_n,
	output reg SRAM_WE_n    
);

	reg [1:0] state = 2'b00;
	reg [15:0] wdata = 16'h0000;
	
	assign SRAM_D = (i_enable && i_rw) ? wdata : 16'hzzzz;
	
	initial begin
		SRAM_A <= 0;
		SRAM_CE_n <= 1;
		SRAM_OE_n <= 1;
		SRAM_WE_n <= 1;
	end

    always @ (posedge i_clock) begin
        if (i_enable) begin
        	if (!i_rw) begin	// read
        		case (state)
        			2'b00:
						begin
							SRAM_A <= i_address >> 2;
							SRAM_CE_n <= 0;
							SRAM_OE_n <= 0;
							state <= 2'b01;
						end
						
        			2'b01:
						begin
							o_rdata[31:16] <= SRAM_D;
							SRAM_CE_n <= 1;
							SRAM_OE_n <= 1;
							state <= 2'b10;
						end
						
        			2'b10:
						begin
							SRAM_A <= (i_address >> 2) + 1;
							SRAM_CE_n <= 0;
							SRAM_OE_n <= 0;
							state <= 2'b11;
						end
						
        			2'b11:
						begin
							o_rdata[15:0] <= SRAM_D;
							o_ready <= 1;
							SRAM_CE_n <= 1;
							SRAM_OE_n <= 1;
						end
        		endcase
        	end
        	else begin	// write
        		case (state)
        			2'b00:
						begin
							wdata <= i_wdata[31:16];
							SRAM_A <= i_address >> 2;
							SRAM_CE_n <= 0;
							SRAM_WE_n <= 0;
							state <= 2'b01;
						end
						
        			2'b01:
						begin
							SRAM_CE_n <= 1;
							SRAM_WE_n <= 1;
							state <= 2'b10;
						end
						
        			2'b10:
						begin
							wdata <= i_wdata[15:0];
							SRAM_A <= (i_address >> 2) + 1;
							SRAM_CE_n <= 0;
							SRAM_WE_n <= 0;
							state <= 2'b11;
						end
						
        			2'b11:
        				begin
							o_ready <= 1;
							SRAM_CE_n <= 1;
							SRAM_WE_n <= 1;
						end
        		endcase
        	end
        end
        else begin
			state <= 2'b00;
			o_ready <= 0;
			SRAM_CE_n <= 1;
			SRAM_OE_n <= 1;
			SRAM_WE_n <= 1;			
        end
    end

endmodule