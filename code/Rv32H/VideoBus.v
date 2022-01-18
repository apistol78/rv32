
`timescale 1ns/1ns

module VideoBus(
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
	reg fifo_write;
	reg [63:0] fifo_wdata;
	reg fifo_read;
	wire [63:0] fifo_rdata;

	FIFO64 #(
		.DEPTH(1024)
	) fifo(
		.i_clock(i_clock),
		.o_empty(fifo_empty),
		.o_full(fifo_full),
		.i_write(fifo_write),
		.i_wdata(fifo_wdata),
		.i_read(fifo_read),
		.o_rdata(fifo_rdata)
	);

	reg [31:0] palette [0:255];
	reg [2:0] state;

	reg [8:0] prefetched_quad_pos_x;
	reg [31:0] prefetched_quad;
	reg [31:0] quad;
	
	wire [1:0] byte_index = i_video_pos_x[1:0];

	initial begin
		o_cpu_ready = 1'b0;
		o_video_rdata = 1'b0;
		o_mem_request = 1'b0;
		o_mem_rw = 1'b0;
		o_mem_address = 32'h0;
		o_mem_wdata = 32'h0;
		o_fifo_full = 1'b0;

		fifo_write = 1'b0;
		fifo_read = 1'b0;
		state = 0;
		
		prefetched_quad_pos_x = 0;
	end

	always @(posedge i_clock) begin
		if (i_cpu_request) begin
			if (i_cpu_address < 32'h01000000) begin
				if (!fifo_full && !fifo_write) begin
					fifo_write <= 1'b1;
					fifo_wdata <= { i_cpu_address, i_cpu_wdata };
					o_cpu_ready <= 1'b1;
				end
			end
			else begin
				palette[(i_cpu_address - 32'h01000000) >> 2] <= i_cpu_wdata;
				o_cpu_ready <= 1'b1;
			end
		end
		else begin
			fifo_write <= 1'b0;
			if (!i_cpu_request)
				o_cpu_ready <= 1'b0;
		end
		o_fifo_full <= fifo_full;
	end
	
	always @(posedge i_clock) begin
		case (byte_index)
		0: o_video_rdata <= palette[quad[7:0]];
		1: o_video_rdata <= palette[quad[15:8]];
		2: o_video_rdata <= palette[quad[23:16]];
		3: o_video_rdata <= palette[quad[31:24]];
		endcase
	end

	// Transfer from FIFO to VRAM.
	// Read VRAM during scan out.
	always @(posedge i_clock) begin
		case (state)
			// Wait until video request or until data in FIFO.
			0: begin
				if (!i_video_request) begin
					o_mem_request <= 1'b0;
					if (!fifo_empty) begin
						fifo_read <= 1'b1;
						state <= 1;
					end
				end
				else begin
					if (i_video_pos_x == prefetched_quad_pos_x) begin
						quad <= prefetched_quad;
					
						o_mem_request <= 1'b1;
						o_mem_rw <= 1'b0;
						
						if (prefetched_quad_pos_x < 316)
							o_mem_address <= ((prefetched_quad_pos_x + 4) + i_video_pos_y * 320); // << 2;
						else
							o_mem_address <= (0 + i_video_pos_y * 320); // << 2;
						
						state <= 3;
					end
				end
			end

			// Copy FIFO to VRAM.
			1: begin
				fifo_read <= 1'b0;
				o_mem_request <= 1'b1;
				o_mem_rw <= 1'b1;
				o_mem_address <= fifo_rdata[63:32];
				o_mem_wdata <= fifo_rdata[31:0];
				state <= 2;
			end

			// Wait until VRAM finished.
			2: begin
				if (i_mem_ready) begin
					o_mem_request <= 1'b0;
					state <= 0;
				end
			end
			
			3: begin
				if (i_mem_ready) begin
					o_mem_request <= 1'b0;
					
					prefetched_quad <= i_mem_rdata;
					
					if (prefetched_quad_pos_x < 316)
						prefetched_quad_pos_x <= prefetched_quad_pos_x + 4;
					else
						prefetched_quad_pos_x <= 0;
					
					state <= 0;
				end
			end
		endcase
	end

endmodule
