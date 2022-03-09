
module GPU(
	input wire i_reset,
	input wire i_clock,

	// Command interface.
	input wire i_request,
	input wire i_rw,
	input wire [1:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output reg o_ready,

	// Framebuffer output.
	output reg o_fb_request,
	input i_fb_ready,
	output reg [31:0] o_fb_address,
	output reg [31:0] o_fb_wdata
);

	reg [31:0] data [0:10];
	reg [7:0] dw = 0;

	logic triangle_request;
	wire triangle_ready;
	wire [9:0] triangle_fb_x;
	wire [9:0] triangle_fb_y;
	wire triangle_fb_wr;

	GPU_Triangle triangle(
    	.i_clock(i_clock),

		.i_request(triangle_request),
		.o_ready(triangle_ready),

    	.i_min({ data[0], data[1] }),
    	.i_max({ data[2], data[3] }),

		.i_v0({ data[4], data[5] }),
		.i_v1({ data[6], data[7] }),
		.i_v2({ data[8], data[9] }),

    	.o_fb_x(triangle_fb_x),
    	.o_fb_y(triangle_fb_y),
    	.o_fb_wr(triangle_fb_wr)
	);

	always @(posedge i_clock) begin

		o_ready <= 0;

		if (i_request) begin
			if (!i_rw) begin
				o_rdata <= 0;
				o_ready <= 1;
			end
			else begin
				if (i_address == 2'b00) begin
					data[dw] <= i_wdata;
					dw <= dw + 1;
					o_ready <= 1;
				end
				else if (i_address == 2'b01) begin
					
					// Execute triangle rasterization.
					triangle_request <= 1;
					if (triangle_ready) begin
						triangle_request <= 0;
						dw <= 0;
						o_ready <= 1;
					end

				end
			end
		end

	end

endmodule
