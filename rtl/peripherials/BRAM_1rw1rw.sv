
`timescale 1ns/1ns

module BRAM_1rw1rw #(
	parameter WIDTH = 32,
	parameter SIZE = 32'h400,
	parameter ADDR_LSH = 2
)(
	input i_clock,

    // Port A (read/write)
	input i_pa_request,
	input i_pa_rw,
	input [31:0] i_pa_address,
	input [WIDTH - 1:0] i_pa_wdata,
	output logic [WIDTH - 1:0] o_pa_rdata,
	output bit o_pa_ready,

    // Port B (read/write)
	input i_pb_request,
	input i_pb_rw,
	input [31:0] i_pb_address,
	input [WIDTH - 1:0] i_pb_wdata,
	output logic [WIDTH - 1:0] o_pb_rdata,
	output bit o_pb_ready
);
    (* ram_style = "block" *)
	logic [WIDTH - 1:0] data [0:SIZE - 1];

	initial begin
        o_pa_ready = 0;
        o_pb_ready = 0;
    end

	always_ff @(posedge i_clock) begin
		o_pa_ready <= 1'b0;
		o_pb_ready <= 1'b0;
		if (i_pa_request) begin
			if (!i_pa_rw) begin
				o_pa_rdata <= data[i_pa_address >> ADDR_LSH];
			end
			else begin
				data[i_pa_address >> ADDR_LSH] <= i_pa_wdata;
			end
			o_pa_ready <= 1'b1;
		end
		if (i_pb_request) begin
			if (!i_pb_rw) begin
				o_pb_rdata <= data[i_pb_address >> ADDR_LSH];
			end
			else begin
				data[i_pb_address >> ADDR_LSH] <= i_pb_wdata;
			end
			o_pb_ready <= 1'b1;
		end
	end

endmodule