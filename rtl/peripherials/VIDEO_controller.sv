
`timescale 1ns/1ns

module VIDEO_controller #(
	parameter MAX_PITCH = 640
)(
	input i_clock,

	// CPU
	input i_cpu_request,
	input i_cpu_rw,
	input [31:0] i_cpu_address,
	input [31:0] i_cpu_wdata,
	output bit [31:0] o_cpu_rdata,
	output bit o_cpu_ready,

	// Video
	input i_video_hblank,
	input i_video_vblank,
	input [10:0] i_video_pos_x,
	input [10:0] i_video_pos_y,
	output bit [31:0] o_video_rdata,

	// Memory
	output bit o_vram_pa_request,
	output bit o_vram_pa_rw,
	output bit [31:0] o_vram_pa_address,
	output bit [31:0] o_vram_pa_wdata,
	input [31:0] i_vram_pa_rdata,
	input i_vram_pa_ready,

	output bit o_vram_pb_request,
	output bit o_vram_pb_rw,
	output bit [31:0] o_vram_pb_address,
	output bit [31:0] o_vram_pb_wdata,
	input [31:0] i_vram_pb_rdata,
	input i_vram_pb_ready
);

	bit [31:0] vram_read_offset = 0;
	bit [31:0] vram_pitch = 0;
	bit [1:0] vram_skip = 0;

	//===============================

	initial begin
		o_cpu_ready = 1'b0;
		o_video_rdata = 1'b0;

		o_vram_pa_request = 1'b0;
		o_vram_pa_rw = 1'b0;
		o_vram_pa_address = 32'h0;
		o_vram_pa_wdata = 32'h0;

		o_vram_pb_request = 1'b0;
		o_vram_pb_rw = 1'b0;
		o_vram_pb_address = 32'h0;
		o_vram_pb_wdata = 32'h0;
	end

	//===============================
	// Palette

	bit palette_cpu_request = 0;
	bit [7:0] palette_cpu_address = 0;
	bit [23:0] palette_cpu_wdata = 0;
	bit [7:0] palette_video_address = 0;
	wire [23:0] palette_video_rdata;

	BRAM_1r1w #(
		.WIDTH(24),
		.SIZE(256),
		.ADDR_LSH(0)
	) palette(
		.i_clock(i_clock),

		// Video read port.
		.i_pa_request(1'b1),
		.i_pa_address(palette_video_address),
		.o_pa_rdata(palette_video_rdata),
		.o_pa_ready(),

		// CPU write port.
		.i_pb_request(palette_cpu_request),
		.i_pb_address({ 24'h0, palette_cpu_address }),
		.i_pb_wdata(palette_cpu_wdata),
		.o_pb_ready()
	);	

	//===============================
	// CPU

	bit [3:0] state = 0;

	always_ff @(posedge i_clock) begin

		o_cpu_ready <= 1'b0;

		unique case (state)
		0: begin
			if (i_cpu_request) begin
				if (i_cpu_address[23:20] == 4'he) begin
					palette_cpu_request <= 1'b1;
					palette_cpu_address <= i_cpu_address[9:2];
					palette_cpu_wdata <= i_cpu_wdata;
					state <= 2;	// access palette
				end
				else if (i_cpu_address[23:20] == 4'hf) begin
					state <= 3;	// access control
				end
				else begin
					o_vram_pa_address <= { 8'b0, i_cpu_address[23:0] };
					o_vram_pa_rw <= i_cpu_rw;
					o_vram_pa_wdata <= i_cpu_wdata;
					o_vram_pa_request <= 1'b1;
					state <= 1;
				end
			end
		end

		// wait on vram.
		1: begin
			if (!i_cpu_request)
				$error("cpu request cancelled unexpectedly");

			if (i_vram_pa_ready) begin
				o_cpu_ready <= 1'b1;
				o_cpu_rdata <= i_vram_pa_rdata;
				o_vram_pa_request <= 1'b0;
				state <= 4;
			end
		end

		// access palette.
		2: begin
			palette_cpu_request <= 1'b0;
			o_cpu_ready <= 1'b1;
			state <= 4;
		end

		// access registers.
		3: begin
			if (i_cpu_address[3:2] == 2'd0) begin
				vram_read_offset <= i_cpu_wdata;
			end
			else if (i_cpu_address[3:2] == 2'd1) begin
				vram_pitch <= i_cpu_wdata;
			end
			else if (i_cpu_address[3:2] == 2'd2) begin
				vram_skip <= i_cpu_wdata[1:0];
			end
			o_cpu_ready <= 1'b1;
			state <= 4;
		end

		// wait until request finishes.
		4: begin
			o_cpu_ready <= i_cpu_request;
			if (!i_cpu_request) begin
				state <= 0;
			end
		end

		endcase
	end

	//===============================
	// Video

	bit line_r_request = 0;
	bit [31:0] line_r_address;
	wire [31:0] line_r_rdata;

	bit line_w_request = 0;
	bit [31:0] line_w_address;
	bit [31:0] line_w_wdata;

	BRAM_1r1w #(
		.WIDTH(32),
		.SIZE(MAX_PITCH / 4),
		.ADDR_LSH(0)
	) line(
		.i_clock(i_clock),
		.i_pa_request(1'b1),
		.i_pa_address(line_r_address),
		.o_pa_rdata(line_r_rdata),
		.o_pa_ready(),
		.i_pb_request(line_w_request),
		.i_pb_address(line_w_address),
		.i_pb_wdata(line_w_wdata),
		.o_pb_ready()
	);

	bit [10:0] column;
	bit [31:0] row;
	bit line_odd_even = 1'b0;
	bit [1:0] hs = 2'b00;
	bit [1:0] vs = 2'b00;

	always_ff @(posedge i_clock) begin

		hs <= { hs[0], i_video_hblank };
		vs <= { vs[0], i_video_vblank };

		// Check if we have entered vblank.
		if (vs == 2'b10) begin
			column <= 0;
			row <= vram_read_offset;
			line_odd_even <= 1'b1;
			o_vram_pb_address <= vram_read_offset;
			o_vram_pb_request <= 1'b1;			
		end

		// Begin read new line.
		if (hs == 2'b10 && i_video_vblank) begin
			if (vram_skip[1] == 1'b0 || line_odd_even) begin
				column <= 0;
				row <= row + vram_pitch;
				o_vram_pb_address <= row + vram_pitch;
				o_vram_pb_request <= 1'b1;
			end
			line_odd_even <= !line_odd_even;
		end

		// Fill line buffer.
		line_w_request <= 1'b0;
		if (o_vram_pb_request) begin
			if (i_vram_pb_ready) begin
				
				line_w_request <= 1'b1;
				line_w_address <= column;
				line_w_wdata <= i_vram_pb_rdata;

				column <= column + 1;

				if (column < vram_pitch / 4) begin
					o_vram_pb_address <= o_vram_pb_address + 4;
					o_vram_pb_request <= 1;
				end
				else begin
					o_vram_pb_request <= 0;
				end
			end
		end
	end

	bit [8:0] pixel_x;
	bit [1:0] switch_x;

	always_comb begin
		if (vram_skip[0] == 1'b0) begin
			pixel_x = i_video_pos_x[10:2];
			switch_x = i_video_pos_x[1:0];
		end
		else begin
			pixel_x = i_video_pos_x[10:3];
			switch_x = i_video_pos_x[2:1];
		end
	end

	always_comb begin
		line_r_address = pixel_x;
	end

	always_comb begin
		unique case (switch_x)
			0: palette_video_address = line_r_rdata[7:0];
			1: palette_video_address = line_r_rdata[15:8];
			2: palette_video_address = line_r_rdata[23:16];
			3: palette_video_address = line_r_rdata[31:24];
		endcase
	end

	always_ff @(posedge i_clock) begin
		o_video_rdata <= { 8'h00, palette_video_rdata };
	end

endmodule
