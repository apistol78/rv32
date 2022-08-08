`timescale 1ns/100ps

module SRAM_interface_tb();

    bit clk;


	wire [19:0] SRAM_A;
	bit SRAM_OE_N;
	bit SRAM_CE_N;
	bit SRAM_WE_N;
	wire [15:0] SRAM_D;

	sram_controller sc(
		.sram_addr_in(SRAM_A),
		.oe_n(SRAM_OE_N),
		.ce_n(SRAM_CE_N),
		.we_n(SRAM_WE_N),
		.sram_data_inout(SRAM_D)
	);


	bit request = 1'b0;
	bit rw = 1'b0;
	bit [31:0] address = 32'h0000_0000;
	bit [31:0] wdata = 32'hffff_ffff;
	wire [31:0] rdata;
	wire ready;


	wire [17:0] sram_a_narrow;

	assign SRAM_A = { 2'b00, sram_a_narrow };

	SRAM_interface sdram_controller_1(
	    .i_reset(1'b0),
	    .i_clock(clk),
	    .i_request(request),
	    .i_rw(rw),
	    .i_address(address),
	    .i_wdata(wdata),
	    .o_rdata(rdata),
	    .o_ready(ready),

	    .SRAM_A(sram_a_narrow),
	    .SRAM_D(SRAM_D),
		.SRAM_CE_n(SRAM_CE_N),
		.SRAM_OE_n(SRAM_OE_N),
		.SRAM_WE_n(SRAM_WE_N),
		.SRAM_LB_n(),
		.SRAM_UB_n()
	);




    initial begin
        clk = 0;
        #1;
        forever clk = #5 ~clk;
    end


    initial begin
        $dumpfile("SRAM_interface_tb.vcd");
        $dumpvars(0, SRAM_interface_tb);

		// init process
		repeat (100) @ (posedge clk);

		// write
		rw <= 1'b1;
		address = 32'h00000000;
		wdata <= 32'hcafe_babe;
		request <= 1'b1;
		while (ready != 1'b1) begin
			@ (posedge clk);
		end
		request <= 1'b0;
		$display("WRITTEN");

		while (ready == 1'b1) begin
			@ (posedge clk);
		end

		// write
		rw <= 1'b1;
		address = 32'h00000004;
		wdata <= 32'hdead_beef;
		request <= 1'b1;
		while (ready != 1'b1) begin
			@ (posedge clk);
		end
		request <= 1'b0;
		$display("WRITTEN");

		while (ready == 1'b1) begin
			@ (posedge clk);
		end

		// read
		rw <= 1'b0;
		address = 32'h00000000;
		request <= 1'b1;
		while (ready != 1'b1) begin
			@ (posedge clk);
		end
		request <= 1'b0;
		$display("READ %x", rdata);

		while (ready == 1'b1) begin
			@ (posedge clk);
		end

		// read
		rw <= 1'b0;
		address = 32'h00000004;
		request <= 1'b1;
		while (ready != 1'b1) begin
			@ (posedge clk);
		end
		request <= 1'b0;
		$display("READ %x", rdata);

		while (ready == 1'b1) begin
			@ (posedge clk);
		end

		// idle
        repeat (100) @ (posedge clk);

        $finish;
    end

endmodule
