
`timescale 1ns/1ns

`define ENABLE_WBUFFER 0

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
	input i_video_hblank,
	input i_video_vblank,
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
	// Write buffer

`ifdef ENABLE_WBUFFER
	bit wbuffer_request = 0;
	wire wbuffer_ready;
	wire [31:0] wbuffer_rdata;

	WriteBuffer #(
		.DEPTH(1024),
		.STALL_READ(0)
	) wbuffer(
		.i_reset(0),
		.i_clock(i_clock),

		.o_empty(),

		.o_bus_rw(o_vram_pa_rw),
		.o_bus_request(o_vram_pa_request),
		.i_bus_ready(i_vram_pa_ready),
		.o_bus_address(o_vram_pa_address),
		.i_bus_rdata(i_vram_pa_rdata),
		.o_bus_wdata(o_vram_pa_wdata),

		.i_rw(i_cpu_rw),
		.i_request(wbuffer_request),
		.o_ready(wbuffer_ready),
		.i_address(i_cpu_address),
		.o_rdata(wbuffer_rdata),
		.i_wdata(i_cpu_wdata)
	);
`endif

	//===============================
	// CPU

	always_ff @(posedge i_clock) begin
		palette_cpu_request <= 0;
		o_cpu_ready <= 0;

`ifdef ENABLE_WBUFFER
		wbuffer_request <= 0;
`else
		o_vram_pa_request <= 0;
`endif

		if (i_cpu_request) begin
			if (i_cpu_address < 32'h00800000) begin
				// Access video memory.
`ifdef ENABLE_WBUFFER
				wbuffer_request <= 1;
				o_cpu_rdata <= wbuffer_rdata;
				o_cpu_ready <= wbuffer_ready;
`else
				o_vram_pa_address <= i_cpu_address;
				o_vram_pa_request <= 1;
				o_vram_pa_rw <= i_cpu_rw;
				o_vram_pa_wdata <= i_cpu_wdata;
				o_cpu_rdata <= i_vram_pa_rdata;
				o_cpu_ready <= i_vram_pa_ready;
`endif
			end
			else if (i_cpu_address < 32'h00810000) begin
				// Access palette.
				palette_cpu_request <= 1;
				palette_cpu_address <= (i_cpu_address - 32'h00800000) >> 2;
				palette_cpu_wdata <= i_cpu_wdata[23:0];
				o_cpu_ready <= 1;
			end
		end
	end	

	//===============================
	// Video

	bit [31:0] line [0:PPITCH/4];
	bit [10:0] column_offset;
	bit [31:0] row_offset;

	bit skip = 0;

	typedef enum bit [1:0]
	{
		WAIT_BLANK = 2'd0,
		WAIT_MEMORY = 2'd1,
		WAIT_DELAY = 2'd2
	}
	state_t;

	state_t read_state = WAIT_BLANK;
	bit [1:0] hs = 2'b00;
	bit [1:0] vs = 2'b00;
	bit hblank = 1'b0;
	bit vblank = 1'b0;

	always_ff @(posedge i_clock) begin

		hs <= { hs[0], i_video_hblank };
		vs <= { vs[0], i_video_vblank };
		
		if (hs == 2'b10) begin
			hblank <= 1'b1;
		end
		if (vs == 2'b10) begin
			vblank <= 1'b1;
		end

		case (read_state)
			WAIT_BLANK: begin
				o_vram_pb_request <= 0;
				if (vblank) begin	// Enter vblank, read first line.
					o_vram_pb_address <= 0;
					o_vram_pb_request <= 1;
					column_offset <= 0;
					row_offset <= 0;
					read_state <= WAIT_MEMORY;
					vblank <= 1'b0;
					hblank <= 1'b0;
					skip <= 1'b1;
				end
				else if (hblank && i_video_vblank) begin			// Enter hblank (within visible region), read next line.
					
					// Skip every other row since we want 320*200 but
					// signal is 640*400.

					if (skip) begin
						o_vram_pb_address <= row_offset + PPITCH;
						o_vram_pb_request <= 1;
						column_offset <= 0;
						row_offset <= row_offset + PPITCH;
						read_state <= WAIT_MEMORY;
					end

					skip <= !skip;
					hblank <= 1'b0;
				end
			end

			WAIT_MEMORY: begin
				if (i_vram_pb_ready) begin
					o_vram_pb_request <= 0;

					line[column_offset] <= i_vram_pb_rdata;
					column_offset <= column_offset + 1;

					if (column_offset < PPITCH / 4) begin
						//read_state <= WAIT_DELAY;
						o_vram_pb_address <= o_vram_pb_address + 4;
						o_vram_pb_request <= 1;
						read_state <= WAIT_MEMORY;
					end
					else begin
						read_state <= WAIT_BLANK;
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
				read_state <= WAIT_BLANK;
			end
		endcase
	end

	always_comb begin
		o_video_rdata = palette_video_rdata;
		case (i_video_pos_x & 3)
			0: palette_video_address = line[i_video_pos_x[9:2]][7:0];
			1: palette_video_address = line[i_video_pos_x[9:2]][15:8];
			2: palette_video_address = line[i_video_pos_x[9:2]][23:16];
			3: palette_video_address = line[i_video_pos_x[9:2]][31:24];
		endcase
	end

endmodule
