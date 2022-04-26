
`timescale 1ns/1ns

module VMODE_chunky #(
	parameter PPITCH = 320,
	parameter [0:0] REGISTERED_CPU_ACCESS = 1'b1
)(
	input i_clock,

	// CPU
	input i_cpu_request,
	input i_cpu_rw,
	input [31:0] i_cpu_address,
	input [31:0] i_cpu_wdata,
	output reg [31:0] o_cpu_rdata,
	output reg o_cpu_ready,

	// Video
	input i_video_request,
	input [8:0] i_video_pos_x,
	input [8:0] i_video_pos_y,
	output reg [31:0] o_video_rdata,

	// Memory
	output reg o_vram_pa_request,
	output reg o_vram_pa_rw,
	output reg [31:0] o_vram_pa_address,
	output reg [31:0] o_vram_pa_wdata,
	input [31:0] i_vram_pa_rdata,
	input i_vram_pa_ready,

	output reg o_vram_pb_request,
	output reg o_vram_pb_rw,
	output reg [31:0] o_vram_pb_address,
	output reg [31:0] o_vram_pb_wdata,
	input [31:0] i_vram_pb_rdata,
	input i_vram_pb_ready
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

	logic [3:0] state = 0;
	logic [31:0] quad [3:0];

	wire [1:0] byte_index = i_video_pos_x[1:0];

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

	generate if (!REGISTERED_CPU_ACCESS) begin

		wire cpu_vram_request = i_cpu_request && (i_cpu_address < 32'h00800000);
		wire cpu_palette_request = i_cpu_request && (i_cpu_address >= 32'h00800000);

		always_comb begin
			o_vram_pa_request = cpu_vram_request;
			o_vram_pa_rw = i_cpu_rw;
			o_vram_pa_address = i_cpu_address;
			o_vram_pa_wdata = i_cpu_wdata;

			palette_cpu_request = cpu_palette_request;
			palette_cpu_wdata = i_cpu_wdata[23:0];
			palette_cpu_address = (i_cpu_address - 32'h00800000) >> 2;

			o_cpu_rdata = i_vram_pa_rdata;
			o_cpu_ready = 
				cpu_vram_request ? i_vram_pa_ready :
				cpu_palette_request ? 1'b1 :
				1'b0;
		end

	end endgenerate

	generate if (REGISTERED_CPU_ACCESS) begin

		always_ff @(posedge i_clock) begin
			palette_cpu_request <= 0;
			o_vram_pa_request <= 0;
			o_cpu_ready <= 0;

			if (i_cpu_request) begin
				if (i_cpu_address < 32'h00800000) begin
					o_vram_pa_address <= i_cpu_address;
					o_vram_pa_request <= 1;
					o_vram_pa_rw <= i_cpu_rw;
					o_vram_pa_wdata <= i_cpu_wdata;
					o_cpu_rdata = i_vram_pa_rdata;
					o_cpu_ready <= i_vram_pa_ready;
				end
				else /*if (i_cpu_address >= 32'h00800000)*/ begin
					palette_cpu_request <= 1;
					palette_cpu_address <= (i_cpu_address - 32'h00800000) >> 2;
					palette_cpu_wdata <= i_cpu_wdata[23:0];
					o_cpu_ready <= 1;
				end
			end
		end

	end endgenerate

	//===============================
	// Video

	always_ff @(posedge i_clock) begin
		case (i_video_pos_x & 3)
		0: o_video_rdata <= quad[0];
		1: o_video_rdata <= quad[1];
		2: o_video_rdata <= quad[2];
		3: o_video_rdata <= quad[3];
		endcase
	end

	always_ff @(posedge i_clock) begin
	
		o_vram_pb_request <= 0;
		o_vram_pb_rw <= 0;
		o_vram_pb_wdata <= 0;
	
		case (state)
			0: begin
				if ((i_video_pos_x & 3) == 0) begin
					o_vram_pb_address <= (i_video_pos_x & ~3) + (i_video_pos_y * PPITCH);
					o_vram_pb_request <= 1;
					state <= 1;
				end
			end

			1: begin
				o_vram_pb_request <= 1;
				if (i_vram_pb_ready) begin
					o_vram_pb_request <= 0;
					palette_video_address = i_vram_pb_rdata[7:0];
					state <= 2;
				end
			end

			2: begin
				quad[0] <= palette_video_rdata;
				palette_video_address = i_vram_pb_rdata[15:8];
				state <= 3;
			end

			3: begin
				quad[1] <= palette_video_rdata;
				palette_video_address = i_vram_pb_rdata[23:16];
				state <= 4;
			end

			4: begin
				quad[2] <= palette_video_rdata;
				palette_video_address = i_vram_pb_rdata[31:24];
				state <= 5;
			end

			5: begin
				quad[3] <= palette_video_rdata;
				state <= 0;
			end
		endcase
	end

endmodule
