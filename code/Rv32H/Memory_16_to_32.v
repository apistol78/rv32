
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
	output reg o_ram_enable,
	output reg o_ram_rw,
	output reg [17:0] o_ram_address,
	output reg [15:0] o_ram_wdata,
	input wire [15:0] i_ram_rdata,
	input wire i_ram_ready
);

	reg [2:0] state;
	
	assign o_ram_clock = i_clock;
	
	initial begin
		state = 0;
		o_ready <= 0;
		o_ram_enable <= 0;
		o_ram_rw <= 0;
	end

	always @ (posedge i_clock) begin
		if (i_enable) begin
			if (!i_rw) begin
				case (state)
					3'd0: begin
						o_ram_enable <= 1;
						o_ram_rw <= 0;
						o_ram_address <= (i_address >> 1) & 32'hfffffffe;
						state <= 3'd1;
					end
						
					3'd1: begin
						if (i_ram_ready) begin
							o_rdata[15:0] <= i_ram_rdata;
							o_ram_enable <= 0;
							state <= 3'd2;
						end
					end

					3'd2: begin
						o_ram_enable <= 1;
						o_ram_rw <= 0;
						o_ram_address <= ((i_address >> 1) & 32'hfffffffe) + 1;
						state <= 3'd3;
					end
						
					3'd3: begin
						if (i_ram_ready) begin
							o_rdata[31:16] <= i_ram_rdata;
							o_ram_enable <= 0;
							o_ready <= 1;
							state <= 3'd4;
						end
					end
				endcase
			end
			else begin
				case (state)	// synopsys full_case
					3'd0: begin
						o_ram_enable <= 1;
						o_ram_rw <= 1;
						o_ram_address <= (i_address >> 1) & 32'hfffffffe;
						o_ram_wdata <= i_wdata[15:0];
						state <= 3'd1;
					end

					3'd1: begin
						if (i_ram_ready) begin
							o_ram_enable <= 0;
							state <= 3'd2;
						end
					end
						
					3'd2: begin
						o_ram_enable <= 1;
						o_ram_rw <= 1;
						o_ram_address <= ((i_address >> 1) & 32'hfffffffe) + 1;
						o_ram_wdata <= i_wdata[31:16];
						state <= 3'd3;
					end

					3'd3: begin
						if (i_ram_ready) begin
							o_ram_enable <= 0;
							o_ready <= 1;
							state <= 3'd4;
						end
					end
				endcase
			end
		end
		else begin
			state <= 3'd0;
			o_ready <= 0;
			o_ram_enable <= 0;
			o_ram_rw <= 0;
		end
	end

endmodule