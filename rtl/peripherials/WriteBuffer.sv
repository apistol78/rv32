
`timescale 1ns/1ns

module WriteBuffer #(
	parameter DEPTH,
	parameter STALL_READ	//!< If set then any read request will stall until fifo is empty.
)(
	input i_reset,
	input i_clock,

	// Control
	output bit o_empty,
	output bit o_full,

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
	typedef enum bit [2:0]
	{
		IDLE = 3'd0,
		WRITE_TO_FIFO = 3'd1,
		READ_FROM_BUS = 3'd2,
		PROCESS_REQUEST = 3'd3,
		TERMINATE_REQUEST = 3'd4
	}
	state_t;

	wire wq_empty;
	wire wq_full;
	bit wq_write = 1'b0;
	bit [63:0] wq_wdata;
	bit wq_read = 1'b0;
	wire [63:0] wq_rdata;

	generate if (DEPTH > 16) begin
		FIFO_BRAM #(
			.DEPTH(DEPTH),
			.WIDTH(64)
		) wq(
			.i_reset(i_reset),
			.i_clock(i_clock),
			.o_empty(wq_empty),
			.o_full(wq_full),
			.i_write(wq_write),
			.i_wdata(wq_wdata),
			.i_read(wq_read),
			.o_rdata(wq_rdata),
			.o_queued()
		);
	end endgenerate

	generate if (DEPTH <= 16) begin
		FIFO #(
			.DEPTH(DEPTH),
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
	end endgenerate

	initial begin
		o_bus_rw = 0;
		o_bus_request = 0;
		o_bus_address = 0;
		o_bus_wdata = 0;
		o_ready = 0;
		o_rdata = 0;
	end

	state_t state = IDLE;
	state_t next_state = IDLE;

	wire stall_read = STALL_READ && !wq_empty;

	always_comb begin
		o_empty = wq_empty;
		o_full = wq_full;
	end

	always_ff @(posedge i_clock) begin
		state <= next_state;
	end

	always_comb begin
		
		next_state = state;

		wq_wdata = 32'h0;
		wq_write = 1'b0;
		wq_read = 1'b0;

		o_bus_request = 1'b0;
		o_bus_rw = 1'b0;
		o_bus_address = 32'h0;
		o_bus_wdata = 32'h0;

		o_rdata = i_bus_rdata;
		o_ready = 1'b0;

		case (state)
			IDLE: begin
				if (i_request) begin
					// Write to fifo; only when not full.
					if (i_rw && !wq_full) begin
						wq_wdata = { i_address, i_wdata };
						wq_write = 1'b1;
						next_state = WRITE_TO_FIFO;
					end
					// Read from bus; only when not stalled.
					else if (!i_rw && !stall_read) begin
						o_bus_request = 1'b1;
						o_bus_rw = 1'b0;
						o_bus_address = i_address;
						next_state = READ_FROM_BUS;
					end
				end

				// No request or not able to process request atm; we
				// attempt to write out to bus from fifo.
				if (!wq_empty) begin
					wq_read = 1'b1;
					next_state = PROCESS_REQUEST;
				end
			end

			WRITE_TO_FIFO: begin
				o_ready = 1'b1;
				if (!i_request)
					next_state = IDLE;
			end

			READ_FROM_BUS: begin
				o_bus_request = 1'b1;
				o_bus_rw = 1'b0;
				o_bus_address = i_address;
				o_ready = i_bus_ready;
				if (!i_request)
					next_state = IDLE;		
			end

			PROCESS_REQUEST: begin
				o_bus_request = 1'b1;
				o_bus_rw = 1'b1;
				o_bus_address = wq_rdata[63:32];
				o_bus_wdata = wq_rdata[31:0];

				// We need to terminate request, ie no bus request for
				// at least one cycle so it won't collide with a pending read
				// request when going back to IDLE.
				if (i_bus_ready)
					next_state = TERMINATE_REQUEST;
			end

			TERMINATE_REQUEST: begin
				next_state = IDLE;
			end

			default: begin
				next_state = IDLE;
			end
		endcase
	end

endmodule
