/*
Bus timing controller.
*/
module Bus(
	input wire i_reset,
	input wire i_clock,

	input wire i_cpu_rw,
	input wire i_cpu_request,
	output reg o_cpu_ready,
	input wire [31:0] i_cpu_address,
	output wire [31:0] o_cpu_rdata,
	input wire [31:0] i_cpu_wdata,

	output wire o_bus_rw,
	output reg o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,
	output wire [31:0] o_bus_wdata
);
	reg [31:0] rdata;
	reg [2:0] state;

	assign o_cpu_rdata = rdata;
	assign o_bus_rw = i_cpu_rw;
	assign o_bus_address = i_cpu_address;
	assign o_bus_wdata = i_cpu_wdata;

	initial begin
		state <= 0;
		o_cpu_ready <= 0;
		o_bus_request <= 0;
	end

	always @(posedge i_clock, posedge i_reset) begin
		if (i_reset) begin
			state <= 0;
			o_cpu_ready <= 0;
			o_bus_request <= 0;
		end
		else begin
			case (state)
				0: begin
					o_cpu_ready <= 0;
					if (i_cpu_request) begin
						state <= 1;
					end                
				end

				1: begin
					o_bus_request <= 1;
					state <= 2;
				end

				2: begin
					if (i_bus_ready) begin
						rdata <= i_bus_rdata;
						o_bus_request <= 0;
						o_cpu_ready <= 1;
						state <= 3;
					end
				end

				3: begin
					if (!i_cpu_request) begin
						o_cpu_ready <= 0;
						state <= 0;
					end 
				end
			endcase
		end
	end

endmodule
