`timescale 1ns / 1ps

module LRU_cache(
	input i_clock,

	// CPU
	input i_request,
	input i_rw,
	input [31:0] i_address,     // Byte address
	input [31:0] i_wdata,
	output logic [31:0] o_rdata,
	output logic o_ready,

	// SDRAM
	output logic o_sdram_request,
	output logic o_sdram_rw,
	output logic [31:0] o_sdram_address,
	output logic [127:0] o_sdram_wdata,
	input [127:0] i_sdram_rdata,
	input i_sdram_ready
);
	typedef enum bit [2:0]
	{
		IDLE,
		WRITE_BACK,
		READ_LINE,
		PROCESS,
		WAIT_END
	}
	state_t;

	state_t state = IDLE;

	logic [27:0] page;
	logic [127:0] line;
	logic valid = 0;
	logic dirty = 0;
	logic ready = 0;

	wire [27:0] input_address_page = i_address[31:4];
	wire [1:0] input_address_dword = i_address[3:2];

	initial begin
		o_ready = 0;
		o_sdram_request = 0;
	end
	
	always_comb begin
	    o_ready = ready && i_request;
	end

	always_ff @(posedge i_clock) begin
		case (state)
			IDLE: begin
				if (i_request) begin
					if (valid && input_address_page == page) begin
						state <= PROCESS;
					end
					else begin
						if (valid && dirty)
							state <= WRITE_BACK;
						else
							state <= READ_LINE;
					end
				end
			end

			WRITE_BACK: begin
				o_sdram_request <= 1;
				o_sdram_rw <= 1;
				o_sdram_address <= { page, 4'b0 };
				o_sdram_wdata <= line;
				if (i_sdram_ready) begin
					o_sdram_request <= 0;
					state <= READ_LINE;
				end
			end

			READ_LINE: begin
				o_sdram_request <= 1;
				o_sdram_rw <= 0;
				o_sdram_address <= { input_address_page, 4'b0 };
				if (i_sdram_ready) begin
					o_sdram_request <= 0;
					page <= input_address_page;
					line <= i_sdram_rdata;
					valid <= 1;
					dirty <= 0;
					state <= PROCESS;
				end
			end

			PROCESS: begin
				if (!i_rw) begin
					case (input_address_dword)
						2'd0: o_rdata <= line[31:0];
						2'd1: o_rdata <= line[63:32];
						2'd2: o_rdata <= line[95:64];
						2'd3: o_rdata <= line[127:96];
					endcase
				end
				else begin
					case (input_address_dword)
						2'd0: line[31:0] <= i_wdata;
						2'd1: line[63:32] <= i_wdata;
						2'd2: line[95:64] <= i_wdata;
						2'd3: line[127:96] <= i_wdata;
					endcase
					dirty <= 1;
				end						
				ready <= 1;
				state <= WAIT_END;			
			end

			WAIT_END: begin
				ready <= 1;
				if (!i_request) begin
					ready <= 0;
					state <= IDLE;
				end
			end

			default:
				state <= IDLE;
		endcase
	end

endmodule
