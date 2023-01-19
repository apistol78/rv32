`timescale 1ns/1ns

module BusValidator #(
    parameter NAME
)(
    input i_clock,
    input i_bus_request,
    input i_bus_ready
);

	bit [31:0] ready_count = 0;
	bit wait_ready = 0;

	always_ff @(posedge i_clock) begin
		ready_count <= 0;
		if (i_bus_request && i_bus_ready) begin
			ready_count <= ready_count + 1;
		end
		if (ready_count >= 2)
			$error("[%s] Bus request held too long", NAME);
//		if (!i_bus_request && i_bus_ready)
//			$error("[%s] Bus ready without request", NAME);

		if (i_bus_request)
			wait_ready <= 1;
		else if (wait_ready)
			$error("[%s] Bus request cancelled before ready", NAME);
		if (i_bus_ready) begin
			assert(wait_ready);
			wait_ready <= 0;
		end
	end

endmodule
