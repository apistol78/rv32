
`timescale 1ns/1ns

`ifdef __VERILATOR__
	`define INSTANT_CLEAR
`endif

module BRAM_clear #(
	parameter WIDTH = 32,
	parameter SIZE = 32'h400,
	parameter ADDR_LSH = 2,
	parameter CLEAR_VALUE = 32'h0
)(
	input i_reset,
	input i_clock,
	output o_initialized,
	input i_request,
	input i_rw,
	input [31:0] i_address,
	input [WIDTH - 1:0] i_wdata,
	output bit [WIDTH - 1:0] o_rdata,
	output bit o_ready
);
    bit [31:0] clear = 0;
    
    assign o_initialized = clear >= SIZE;

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			clear <= 0;
		end
		else if (clear < SIZE) begin
			clear <= clear + 1;
		end
	end
	
	logic ready;
	
    BRAM #(
        .WIDTH(WIDTH),
        .SIZE(SIZE),
        .ADDR_LSH(ADDR_LSH)
    ) bram(
        .i_clock(i_clock),
        .i_request(o_initialized ? i_request : 1'b1),
        .i_rw(o_initialized ? i_rw : 1'b1),
        .i_address(o_initialized ? i_address : clear),
        .i_wdata(o_initialized ? i_wdata : CLEAR_VALUE),
        .o_rdata(o_rdata),
        .o_ready(ready)
    );
    
    assign o_ready = o_initialized ? ready : 1'b0;
    
/*
    (* ram_style = "block" *)
	reg [WIDTH - 1:0] data [0:SIZE - 1];
	reg [31:0] clear;

	integer i;
	initial begin
		o_ready = 0;
		o_rdata = 0;
`ifdef INSTANT_CLEAR
		clear = SIZE;
		for (i = 0; i < SIZE; i = i + 1)
			data[i] = CLEAR_VALUE;
`else
		clear = 0;
`endif
	end

	assign o_initialized = clear >= SIZE;

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			clear <= 0;
		end
		else if (clear < SIZE) begin
			clear <= clear + 1;
		end
	end

	always_ff @(posedge i_clock) begin
		if (clear < SIZE) begin
			data[clear] <= CLEAR_VALUE;
		end 
		else begin
			if (i_request) begin
				if (!i_rw) begin
					o_rdata <= data[i_address >> ADDR_LSH];
				end
				else begin
					data[i_address >> ADDR_LSH] <= i_wdata;
				end
			end
		end
	end

	always_ff @(posedge i_clock)
		o_ready <= i_request;
*/
endmodule