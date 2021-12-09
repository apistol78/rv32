module ap_spi_master(
	input wire iCLK,

	input wire iEN,
	output reg oRDY,
	input wire [7:0] iTX,
	output reg [7:0] oRX,

	output reg oSCK,
	output reg oMOSI,
	input wire iMISO
);

	reg [15:0] step;
	reg iEN_last;

	initial begin
		step <= 0;
		iEN_last <= 1'b0;

		oRDY <= 1'b0;
		oRX <= 8'h00;
		oSCK <= 1'b0;
		oMOSI <= 1'b1;
	end

	always @ (posedge iCLK) begin
		if (iEN) begin
			if (!iEN_last) begin
				oRDY <= 1'b0;
				oRX <= 8'h00;
				oSCK <= 1'b0;
				step <= 0;
			end
			else if (step <= 16'd31) begin
				case (step & 16'd3)
				0: begin
					oSCK <= 1'b0;
					oMOSI <= iTX[7 - (step >> 2)];
				end
				1: begin
					oSCK <= 1'b1;
				end
				2: begin
					oSCK <= 1'b1;
					oRX[7 - (step >> 2)] <= iMISO;
				end
				3: begin
					oSCK <= 1'b0;
				end
				endcase
				step <= step + 1;
			end
			else if (step <= 16'd32) begin
				step <= step + 16'd1;
			end
			else begin
				oRDY <= 1'b1;
				oMOSI <= 1'b1;
			end
		end
		else begin
			if (iEN_last) begin
				oRDY <= 1'b0;
			end
		end
		iEN_last <= iEN;
	end

endmodule 