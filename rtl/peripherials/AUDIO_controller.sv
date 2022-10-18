
`timescale 1ns/1ns

module AUDIO_controller #(
	parameter BUFFER_SIZE = 4096
)(
	input i_reset,
	input i_clock,

	// CPU interface.
	input i_request,
	input i_rw,
	input [15:0] i_wdata,
	output bit [31:0] o_rdata,
	output bit o_ready,
	output bit o_interrupt,

	// Audio output
	input i_output_busy,
	output [15:0] o_output_sample
);

    wire output_fifo_empty;
	wire output_fifo_full;
	bit output_fifo_wr = 0;
	bit output_fifo_rd = 0;
	wire [$clog2(BUFFER_SIZE)-1:0] output_fifo_queued;
	FIFO_BRAM #(
		.DEPTH(BUFFER_SIZE),
		.WIDTH(16)
	) output_fifo(
		.i_reset(i_reset),
        .i_clock(i_clock),
        .o_empty(output_fifo_empty),
		.o_full(output_fifo_full),
		.i_write(output_fifo_wr),
		.i_wdata(i_wdata),
		.i_read(output_fifo_rd),
		.o_rdata(o_output_sample),
		.o_queued(output_fifo_queued)
	);

    initial begin
		o_ready = 0;
		o_interrupt = 0;
	end

	always_ff @(posedge i_clock) begin
		output_fifo_wr <= 0;
		if (i_request && !o_ready) begin
			if (!i_rw) begin
				o_rdata <= output_fifo_queued;
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

	bit [1:0] last_queued = 2'b00;
	always_ff @(posedge i_clock) begin
		last_queued <= { last_queued[0], output_fifo_queued > BUFFER_SIZE / 2 };
	end

	always_comb begin
		o_interrupt = (last_queued == 2'b10);
	end

endmodule
