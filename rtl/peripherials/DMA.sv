
`timescale 1ns/1ns

module DMA(
	input i_reset,
	input i_clock,

	input i_request,
	input i_rw,
	input [1:0] i_address,
	input [31:0] i_wdata,
	output bit [31:0] o_rdata,
	output bit o_ready,

	// System
	input i_stall,

	// Bus
	output bit o_bus_rw,				// Data read/write
	output bit o_bus_request,			// IO request.
	input i_bus_ready,					// IO request ready.
	output bit [31:0] o_bus_address,	// Address
	input [31:0] i_bus_rdata,			// Read data
	output bit [31:0] o_bus_wdata		// Write data,
);
	typedef enum bit [3:0]
	{
		IDLE			= 4'd0,
		READ_CMD		= 4'd1,

		// Write
		W_WRITE_REQ		= 4'd2,
		W_WAIT_WRITE	= 4'd3,

		// Copy
		C_READ_REQ		= 4'd4,
		C_WAIT_READ		= 4'd5,
		C_WRITE_REQ		= 4'd6,
		C_WAIT_WRITE	= 4'd7
	}
	state_t;

	typedef enum bit [1:0]
	{
		WRITE	= 2'd1,
		COPY	= 2'd2
	}
	dma_type_t;

	typedef struct packed
	{
		dma_type_t dt;
		bit [31:0] value_or_from;
		bit [31:0] to;
		bit [31:0] count;
	}
	dma_command_t;

	wire queue_empty;
	wire queue_full;
	bit queue_write = 0;
	bit queue_read = 0;
	dma_command_t queue_rdata;
	FIFO #(
		.DEPTH(8),
		.WIDTH($bits(dma_command_t))
	) queue(
		.i_clock(i_clock),
		.o_empty(queue_empty),
		.o_full(queue_full),
		.i_write(queue_write),
		.i_wdata(wr_command),
		.i_read(queue_read),
		.o_rdata(queue_rdata),
		.o_queued()
	);

	dma_command_t wr_command;
	dma_command_t rd_command;
	bit [31:0] data;
	state_t state = IDLE;

	initial begin
		o_ready = 0;
		o_bus_rw = 0;
		o_bus_request = 0;
		o_bus_address = 0;
		o_bus_wdata = 0;
	end

	// Receive commands and insert into queue.
	always_ff @(posedge i_clock) begin
		queue_write <= 0;
		if (i_request) begin
			if (!i_rw) begin
				if (i_address == 2'b11) begin
					o_rdata <= (!queue_empty || state != IDLE) ? 32'hffff_ffff : 32'h0000_0000;
					o_ready <= 1;
				end
			end
			else begin
				// Receive commands from CPU.
				if (i_address == 2'b00) begin
					wr_command.value_or_from <= i_wdata;
					o_ready <= 1;
				end
				else if (i_address == 2'b01) begin
					wr_command.to <= i_wdata;
					o_ready <= 1;
				end
				else if (i_address == 2'b10) begin
					wr_command.count <= i_wdata;
					o_ready <= 1;
				end
				else if (i_address == 2'b11) begin
					wr_command.dt <= dma_type_t'(i_wdata[1:0]);
					if (!queue_full) begin
						queue_write <= !o_ready;
						o_ready <= 1;
					end
				end
			end
		end
		else
			o_ready <= 0;
	end

	// Process commands.
	always_ff @(posedge i_clock) begin
		queue_read <= 0;
		case (state)
			IDLE: begin
				if (!queue_empty) begin
					queue_read <= 1;
					state <= READ_CMD;
				end
			end

			READ_CMD: begin
				rd_command <= queue_rdata;
				if (queue_rdata.dt == WRITE)
					state <= W_WRITE_REQ;
				else if (queue_rdata.dt == COPY)
					state <= C_READ_REQ;
				else
					state <= IDLE;
			end

			// Write

			W_WRITE_REQ: begin
				if (!i_stall) begin
					o_bus_request <= 1;
					o_bus_rw <= 1;
					o_bus_address <= rd_command.to;
					o_bus_wdata <= rd_command.value_or_from;
					state <= W_WAIT_WRITE;
				end				
			end

			W_WAIT_WRITE: begin
				if (i_bus_ready) begin
					o_bus_request <= 0;
					rd_command.to <= rd_command.to + 4;
					if (rd_command.count > 0) begin
						rd_command.count <= rd_command.count - 1;
						state <= W_WRITE_REQ;
					end
					else begin
						state <= IDLE;
					end
				end
			end

			// Copy

			C_READ_REQ: begin
				if (!i_stall) begin
					o_bus_request <= 1;
					o_bus_rw <= 0;
					o_bus_address <= rd_command.value_or_from;
					state <= C_WAIT_READ;
				end
			end

			C_WAIT_READ: begin
				if (i_bus_ready) begin
					o_bus_request <= 0;
					data <= i_bus_rdata;
					rd_command.value_or_from <= rd_command.value_or_from + 4;
					state <= C_WRITE_REQ;
				end
			end

			C_WRITE_REQ: begin
				if (!i_stall) begin
					o_bus_request <= 1;
					o_bus_rw <= 1;
					o_bus_address <= rd_command.to;
					o_bus_wdata <= data;
					state <= C_WAIT_WRITE;
				end
			end

			C_WAIT_WRITE: begin
				if (i_bus_ready) begin
					o_bus_request <= 0;
					rd_command.to <= rd_command.to + 4;
					if (rd_command.count > 0) begin
						rd_command.count <= rd_command.count - 1;
						state <= C_READ_REQ;
					end
					else begin
						state <= IDLE;
					end
				end
			end

			default:
				state <= IDLE;
		endcase
	end

endmodule
