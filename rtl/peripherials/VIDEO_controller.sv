
`timescale 1ns/1ns

module VIDEO_controller #(
	parameter PPITCH = 320
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
	input i_video_hsync,
	input i_video_vsync,
	input i_video_request,
	input [9:0] i_video_pos_x,
	input [9:0] i_video_pos_y,
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

	bit palette_cpu_request = 0;
	bit [7:0] palette_cpu_address = 0;
	bit [23:0] palette_cpu_wdata = 0;
	bit [7:0] palette_video_address = 0;
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

	bit [31:0] cpu_offset = 0;
	bit [31:0] vram_offset = 0;

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
	// CPU

	always_ff @(posedge i_clock) begin
		palette_cpu_request <= 0;
		o_vram_pa_request <= 0;
		o_cpu_ready <= 0;

		if (i_cpu_request) begin
			if (i_cpu_address < 32'h00800000) begin
				// Access video memory.
				o_vram_pa_address <= cpu_offset + i_cpu_address;
				o_vram_pa_request <= 1;
				o_vram_pa_rw <= i_cpu_rw;
				o_vram_pa_wdata <= i_cpu_wdata;
				o_cpu_rdata <= i_vram_pa_rdata;
				o_cpu_ready <= i_vram_pa_ready;
			end
			else if (i_cpu_address < 32'h00810000) begin
				// Access palette.
				palette_cpu_request <= 1;
				palette_cpu_address <= (i_cpu_address - 32'h00800000) >> 2;
				palette_cpu_wdata <= i_cpu_wdata[23:0];
				o_cpu_ready <= 1;
			end
			else begin
				// Access control registers.
				if (i_cpu_address[3:0] == 0)
					vram_offset <= i_cpu_wdata;
				else if (i_cpu_address[3:0] == 4)
					cpu_offset <= i_cpu_wdata;
				o_cpu_ready <= 1;
			end
		end
	end	

	//===============================
	// Video

	bit [31:0] line [0:PPITCH/4];
	bit [10:0] count;

	typedef enum bit [1:0]
	{
		WAIT_HBLANK = 2'd0,
		WAIT_MEMORY = 2'd1,
		WAIT_DELAY = 2'd2
	}
	state_t;

	state_t read_state = WAIT_HBLANK;

	bit [1:0] hs = 2'b00;
	always_ff @(posedge i_clock) begin
		hs <= { hs[0], i_video_hsync };
	end
	
	bit [1:0] vs = 2'b00;
	always_ff @(posedge i_clock) begin
		vs <= { vs[0], i_video_vsync };
	end

	always_ff @(posedge i_clock) begin
		case (read_state)
			WAIT_HBLANK: begin
				if (hs == 2'b01 && (vs == 2'b00 || vs == 2'b01)) begin
					o_vram_pb_address <= /*vram_offset +*/ (i_video_pos_y * PPITCH);
					o_vram_pb_request <= 1;
					count <= 0;
					read_state <= WAIT_MEMORY;					
				end
			end

			WAIT_MEMORY: begin
				if (i_vram_pb_ready) begin
					o_vram_pb_request <= 0;

					line[count] <= i_vram_pb_rdata;
					count <= count + 1;

					if (count < PPITCH/4-2) begin
						read_state <= WAIT_DELAY;					
					end
					else begin
						read_state <= WAIT_HBLANK;
					end
				end
			end

			WAIT_DELAY: begin
				if (!i_vram_pb_ready) begin
					o_vram_pb_address <= o_vram_pb_address + 4;
					o_vram_pb_request <= 1;
					read_state <= WAIT_MEMORY;
				end
			end

			default: begin
				read_state <= WAIT_HBLANK;
			end
		endcase
	end

	always_comb begin
		o_video_rdata = i_video_request ? palette_video_rdata : 32'h0;
		case (i_video_pos_x & 3)
			0: palette_video_address = line[i_video_pos_x[9:2]][7:0];
			1: palette_video_address = line[i_video_pos_x[9:2]][15:8];
			2: palette_video_address = line[i_video_pos_x[9:2]][23:16];
			3: palette_video_address = line[i_video_pos_x[9:2]][31:24];
		endcase
	end

endmodule
