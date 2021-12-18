
// 16-bit memory to 32-bit memory interface.
module Memory_16_to_32(
    input wire i_clock,
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
    output reg o_ready,

    // 16-bit device.
    output wire o_ram_clock,
	output wire o_ram_enable,
	output wire o_ram_rw,
	output reg [17:0] o_ram_address,
	output wire [15:0] o_ram_wdata,
	input wire [15:0] i_ram_rdata,
	input wire i_ram_ready
);

	reg [2:0] state = 0;
	reg [15:0] wdata = 0;
	
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
        			3'd0: begin
						SRAM_A <= i_address >> 2;
						SRAM_CE_n <= 0;
						SRAM_OE_n <= 0;
						state <= 3'd1;
					end
						
        			3'd1: begin
						o_rdata[15:0] <= SRAM_D;
						SRAM_CE_n <= 1;
						SRAM_OE_n <= 1;
						state <= 3'd2;
					end
						
        			3'd2: begin
						SRAM_A <= (i_address >> 2) + 1;
						SRAM_CE_n <= 0;
						SRAM_OE_n <= 0;
						state <= 3'd3;
					end
						
        			3'd3: begin
						o_rdata[31:16] <= SRAM_D;
						o_ready <= 1;
						SRAM_CE_n <= 1;
						SRAM_OE_n <= 1;
						state <= 3'd4;
					end

					3'd4: begin
						// Nothing, wait for i_enable to reset.
					end
        		endcase
        	end
        	else begin	// write
        		case (state)
        			3'd0: begin
						wdata <= i_wdata[15:0];
						SRAM_A <= i_address >> 2;
						SRAM_CE_n <= 0;
						SRAM_WE_n <= 0;
						state <= 3'd1;
					end

        			3'd1: begin
						SRAM_CE_n <= 1;
						SRAM_WE_n <= 1;
						state <= 3'd2;
					end
						
        			3'd2: begin
						wdata <= i_wdata[31:16];
						SRAM_A <= (i_address >> 2) + 1;
						SRAM_CE_n <= 0;
						SRAM_WE_n <= 0;
						state <= 3'd3;
					end

        			3'd3: begin
						SRAM_CE_n <= 1;
						SRAM_WE_n <= 1;
						state <= 3'd4;
						o_ready <= 1;
					end

					3'd4: begin
						// Nothing, wait for i_enable to reset.
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