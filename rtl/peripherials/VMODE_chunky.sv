
`timescale 1ns/1ns

module VMODE_chunky #(
	PPITCH = 320,
	FIFO_DEPTH = 256
)(
	input wire i_clock,

	// CPU
	input wire i_cpu_request,
	input wire [31:0] i_cpu_address,
	input wire [31:0] i_cpu_wdata,
	output reg o_cpu_ready,

	// Video
	input wire i_video_request,
	input wire [8:0] i_video_pos_x,
	input wire [8:0] i_video_pos_y,
	output reg [31:0] o_video_rdata,

	// Memory
	output reg o_mem_request,
	output reg o_mem_rw,
	output reg [31:0] o_mem_address,
	output reg [31:0] o_mem_wdata,
	input wire [31:0] i_mem_rdata,
	input wire i_mem_ready,
	
	// Debug
	output reg o_fifo_full
);

	wire fifo_empty;
	wire fifo_full;
	logic fifo_write = 0;
	logic [63:0] fifo_wdata;
	logic fifo_read = 0;
	wire [63:0] fifo_rdata;

	FIFO64 #(
		.DEPTH(FIFO_DEPTH)
	) fifo(
		.i_clock(i_clock),
		.o_empty(fifo_empty),
		.o_full(fifo_full),
		.i_write(fifo_write),
		.i_wdata(fifo_wdata),
		.i_read(fifo_read),
		.o_rdata(fifo_rdata)
	);

	logic palette_cpu_request = 0;
	logic [7:0] palette_cpu_address = 0;
	logic [23:0] palette_cpu_wdata = 0;
	logic [7:0] palette_video_address = 0;
	wire [23:0] palette_video_rdata;

	BRAM_dual #(
		.WIDTH(24),
		.SIZE(256),
		.ADDR_LSH(0)
	) palette(
		.i_clock(i_clock),
		
		// CPU write port.
		.i_pa_request(palette_cpu_request),
		.i_pa_rw(1'b1),
		.i_pa_address({ 24'h0, palette_cpu_address }),
		.i_pa_wdata(palette_cpu_wdata),
		.o_pa_rdata(),
		.o_pa_ready(),

		// Video read port.
		.i_pb_request(1'b1),
		.i_pb_rw(1'b0),
		.i_pb_address(palette_video_address),
		.i_pb_wdata(0),
		.o_pb_rdata(palette_video_rdata),
		.o_pb_ready()
	);

	logic [2:0] state = 0;
	logic [31:0] quad = 0;
	logic [31:0] next_quad = 0;
	
	wire [1:0] byte_index = i_video_pos_x[1:0];

	initial begin
		o_cpu_ready = 1'b0;
		o_video_rdata = 1'b0;
		o_mem_request = 1'b0;
		o_mem_rw = 1'b0;
		o_mem_address = 32'h0;
		o_mem_wdata = 32'h0;
		o_fifo_full = 1'b0;
	end

	always_ff @(posedge i_clock) begin
		if (i_cpu_request && !o_cpu_ready) begin
			if (i_cpu_address < 32'h01000000) begin
				if (!fifo_full) begin
					fifo_write <= 1'b1;
					fifo_wdata <= { i_cpu_address, i_cpu_wdata };
					o_cpu_ready <= 1'b1;
				end
			end
			else begin
				palette_cpu_request <= 1'b1;
				palette_cpu_wdata <= i_cpu_wdata[23:0];
				palette_cpu_address <= (i_cpu_address - 32'h01000000) >> 2;
				o_cpu_ready <= 1'b1;
			end
		end
		else begin
			fifo_write <= 1'b0;
			palette_cpu_request <= 1'b0;
			o_cpu_ready <= 1'b0;
		end
		o_fifo_full <= fifo_full;
	end

	always_comb begin
		palette_video_address = 0;
		o_video_rdata = 0;
		if (i_video_request) begin
			case (byte_index)
				0: palette_video_address = quad[7:0];
				1: palette_video_address = quad[15:8];
				2: palette_video_address = quad[23:16];
				3: palette_video_address = quad[31:24];
			endcase
			o_video_rdata = palette_video_rdata;
		end
	end
	
	always_ff @(posedge i_clock) begin
	 	if ((i_video_pos_x & 3) == 0)
	 		quad <= next_quad;
	 end
	
	// Transfer from FIFO to VRAM.
	// Read VRAM during scan out.
	always_ff @(posedge i_clock) begin
	
		o_mem_request <= 0;
		o_mem_rw <= 0;
		o_mem_address <= 0;
		o_mem_wdata <= 0;
		
		fifo_read <= 0;
	
		case (state)
			// Wait until video request or until data in FIFO.
			0: begin
				if (!i_video_request) begin
					if (!fifo_empty) begin
						fifo_read <= 1;
						state <= 1;
					end
				end
				else begin
					if (((i_video_pos_x + 2) & 3) == 0) begin
						o_mem_request <= 1;
						o_mem_address <= ((i_video_pos_x + 2) & ~3) + (i_video_pos_y * PPITCH);
						state <= 2;
					end
				end
			end

			// Copy FIFO to VRAM.
			1: begin
				o_mem_request <= 1;
				o_mem_rw <= 1;
				o_mem_address <= fifo_rdata[63:32];
				o_mem_wdata <= fifo_rdata[31:0];
				if (i_mem_ready) begin
					o_mem_request <= 0;
					state <= 0;
				end
			end

			2: begin
				o_mem_request <= 1;
				o_mem_address <= ((i_video_pos_x + 2) & ~3) + (i_video_pos_y * PPITCH);
				if (i_mem_ready) begin
					o_mem_request <= 0;
					next_quad <= i_mem_rdata;
					state <= 3;
				end
			end

			3: begin
				if (((i_video_pos_x + 1) & 3) == 3)
					state <= 0;
			end
		endcase
	end

endmodule
