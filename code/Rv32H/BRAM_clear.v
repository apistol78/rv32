
`timescale 1ns/1ns

module BRAM_clear #(
	parameter WIDTH = 32,
	parameter SIZE = 32'h400,
	parameter ADDR_LSH = 2
)(
	input wire i_clock,
    output wire o_initialized,
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [WIDTH - 1:0] i_wdata,
	output reg [WIDTH - 1:0] o_rdata,
	output reg o_ready
);

	reg [WIDTH - 1:0] data [0:SIZE - 1];
    reg [31:0] clear;

	initial begin
        o_ready = 0;
        clear = 0;
    end

    assign o_initialized = clear >= SIZE;

	always @(posedge i_clock) begin
        if (clear < SIZE) begin
            data[clear] <= 0;
            clear <= clear + 1;
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

	always @(posedge i_clock)
		o_ready <= i_request;

endmodule