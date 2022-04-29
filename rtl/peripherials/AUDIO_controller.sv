
`timescale 1ns/1ns

module AUDIO_controller(
	input i_reset,
	input i_clock,

	// CPU interface.
	input i_request,
	input i_rw,
	input [15:0] i_wdata,
	output logic [31:0] o_rdata,
	output logic o_ready,

	// Audio output
	input i_output_busy,
	output [15:0] o_output_sample
);

    wire output_fifo_empty;
	wire output_fifo_full;
	logic output_fifo_wr = 0;
	logic output_fifo_rd = 0;
	wire [7:0] output_fifo_queued;
	FIFO64 #(
		.DEPTH(256),
		.WIDTH(16)
	) output_fifo(
        .i_clock(i_clock),
        .o_empty(output_fifo_empty),
		.o_full(output_fifo_full),
		.i_write(output_fifo_wr),
		.i_wdata(i_wdata),
		.i_read(output_fifo_rd),
		.o_rdata(o_output_sample),
		.o_queued(output_fifo_queued)
	);

    initial o_ready = 0;

	always_ff @(posedge i_clock) begin
		output_fifo_wr <= 0;
		if (i_request && !o_ready) begin
			if (!i_rw) begin
				o_rdata <= { 24'b0, output_fifo_queued };
				o_ready <= 1;
			end
			else begin
				if (!output_fifo_full) begin
					output_fifo_wr <= 1;
					o_ready <= 1;
				end
			end
		end
		else if (!i_request) begin
			o_ready <= 0;
		end
	end

	always_ff @(posedge i_clock) begin
		output_fifo_rd <= !i_output_busy && !output_fifo_empty;
	end

endmodule
