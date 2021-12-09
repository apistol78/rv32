module ap_dual_register(
	input wire iW_0,
	input wire [7:0] iDATA_0,
	input wire iW_1,
	input wire [7:0] iDATA_1,
	output wire [7:0] oDATA
);
	reg [7:0] data = 8'h00;

	assign oDATA = data;

	always @ (iW_0 or iW_1) begin
		if (iW_0) 
			data <= iDATA_0;
		if (iW_1) 
			data <= iDATA_1;
	end

endmodule