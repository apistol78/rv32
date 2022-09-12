
`timescale 1ns/1ns

module WriteBuffer(
	input i_reset,
	input i_clock,

	// Bus
	output bit o_bus_rw,
	output bit o_bus_request,
	input i_bus_ready,
	output bit [31:0] o_bus_address,
	input [31:0] i_bus_rdata,
	output bit [31:0] o_bus_wdata,

	// Input
	input i_rw,
	input i_request,
	output bit o_ready,
	input [31:0] i_address,
	output logic [31:0] o_rdata,
	input [31:0] i_wdata
);

	wire wq_empty;
	wire wq_full;
	bit wq_write = 1'b0;
	bit [63:0] wq_wdata;
	bit wq_read = 1'b0;
	wire [63:0] wq_rdata;

	FIFO #(
		.DEPTH(64),
		.WIDTH(64)
	) wq(
		.i_clock(i_clock),
		.o_empty(wq_empty),
		.o_full(wq_full),
		.i_write(wq_write),
		.i_wdata(wq_wdata),
		.i_read(wq_read),
		.o_rdata(wq_rdata),
		.o_queued()
	);

	initial begin
		o_bus_rw = 0;
		o_bus_request = 0;
		o_bus_address = 0;
		o_bus_wdata = 0;
		o_ready = 0;
		o_rdata = 0;
	end

	bit [1:0] state = 0;
	bit [1:0] next_state = 0;

	bit [31:0] stall = 0;
	bit [31:0] next_stall = 0;

	always_ff @(posedge i_clock) begin
		state <= next_state;
		stall <= next_stall;
	end

	always_comb begin
		
		next_state = state;
		next_stall = stall;

		wq_wdata = 32'h0;
		wq_write = 1'b0;

		wq_read = 1'b0;

		o_bus_request = 1'b0;
		o_bus_rw = 1'b0;
		o_bus_address = 32'h0;
		o_bus_wdata = 32'h0;

		o_rdata = (i_request && !i_rw) ? i_bus_rdata : 32'hz;
		o_ready = 1'b0;

		case (state)
			0: begin
				if (i_request) begin
					if (i_rw && !wq_full) begin
						wq_wdata = { i_address, i_wdata };
						wq_write = 1'b1;
						o_ready = 1'b1;
						next_state = 1;
					end
					else if (!i_rw && wq_empty) begin
						o_bus_request = 1'b1;
						o_bus_rw = 1'b0;
						o_bus_address = i_address;
						next_state = 2;
					end
					else if (!i_rw && !wq_empty) begin
						next_stall = stall + 1;
					end
				end

				if (!wq_empty) begin
					wq_read = 1'b1;
					next_state = 3;
				end
			end

			1: begin
				wq_wdata = { i_address, i_wdata };
				wq_write = 1'b1;
				o_ready = 1'b1;
				if (!i_request)
					next_state = 0;
			end

			2: begin
				o_bus_request = 1'b1;
				o_bus_rw = 1'b0;
				o_bus_address = i_address;
				o_ready = i_bus_ready;
				if (!i_request)
					next_state = 0;		
			end

			3: begin
				o_bus_request = 1'b1;
				o_bus_rw = 1'b1;
				o_bus_address = wq_rdata[63:32];
				o_bus_wdata = wq_rdata[31:0];
				o_ready = i_bus_ready;
				if (!i_request)
					next_state = 0;
			end

		endcase

	end

endmodule
