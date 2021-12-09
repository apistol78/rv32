module ap_delay(
	input wire iCLK,
	input wire iENABLE,
	output reg oFINISHED
);

	reg enableN1 = 1'b0;
	reg [31:0] counter;

	always @ (posedge iCLK) begin
		if (iENABLE && !enableN1) begin
			counter <= 200000;
			oFINISHED <= 1'b0;
		end else if (iENABLE) begin
			if (counter > 0) begin
				counter <= counter - 1;
			end else begin
				oFINISHED <= 1'b1;
			end
		end else if (!iENABLE) begin
			oFINISHED <= 1'b0;
		end
		enableN1 <= iENABLE;
	end

endmodule
