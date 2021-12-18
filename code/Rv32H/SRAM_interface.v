
// SRAM interface
module SRAM_interface(
    input wire i_clock,
	input wire i_enable,
	input wire i_rw,
	input wire [17:0] i_address,
	input wire [15:0] i_wdata,
	output reg [15:0] o_rdata,
    output reg o_ready,

	output reg [17:0] SRAM_A,
	inout wire [15:0] SRAM_D,
	output reg SRAM_CE_n,
	output reg SRAM_OE_n,
	output reg SRAM_WE_n    
);

	reg [2:0] state = 0;
	
	assign SRAM_D = (i_enable && i_rw) ? i_wdata : 16'hzzzz;
	
	initial begin
		SRAM_CE_n <= 1;
		SRAM_OE_n <= 1;
		SRAM_WE_n <= 1;
	end

    always @ (posedge i_clock) begin
        if (i_enable) begin
        	if (!i_rw) begin	// read
        		case (state)
        			3'd0: begin
						SRAM_A <= i_address;
						SRAM_CE_n <= 0;
						SRAM_OE_n <= 0;
						state <= 3'd1;
					end
						
        			3'd1: begin
						o_rdata <= SRAM_D;
						o_ready <= 1;
						SRAM_CE_n <= 1;
						SRAM_OE_n <= 1;
						state <= 3'd2;
					end
        		endcase
        	end
        	else begin	// write
        		case (state)
        			3'd0: begin
						SRAM_A <= i_address;
						SRAM_CE_n <= 0;
						SRAM_WE_n <= 0;
						state <= 3'd1;
					end

        			3'd1: begin
						o_ready <= 1;
						SRAM_CE_n <= 1;
						SRAM_WE_n <= 1;
						state <= 3'd2;
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