`timescale 1ns/1ns

module SDRAM_controller_tb();

    bit clk;
	bit clk_sdram;


	// wire [15:0] m_Dq_1;
	// bit [12:0] m_Addr_1;
	// bit [1:0] m_Ba_1;
	// bit m_Clk_1;
	// bit m_Cke_1;
	// bit m_Cs_n_1;
	// bit m_Ras_n_1;
	// bit m_Cas_n_1;
	// bit m_We_n_1;
	// bit [1:0] m_Dqm_1;

	// mt48lc16m16a2 m_1(
	// 	.Dq(m_Dq_1),
	// 	.Addr(m_Addr_1),
	// 	.Ba(m_Ba_1),
	// 	.Clk(m_Clk_1),
	// 	.Cke(m_Cke_1),
	// 	.Cs_n(m_Cs_n_1),
	// 	.Ras_n(m_Ras_n_1),
	// 	.Cas_n(m_Cas_n_1),
	// 	.We_n(m_We_n_1),
	// 	.Dqm(m_Dqm_1)
	// );


	wire [15:0] m_Dq_2;
	bit [12:0] m_Addr_2;
	bit [1:0] m_Ba_2;
	bit m_Clk_2;
	bit m_Cke_2;
	bit m_Cs_n_2;
	bit m_Ras_n_2;
	bit m_Cas_n_2;
	bit m_We_n_2;
	bit [1:0] m_Dqm_2;

	mt48lc16m16a2 m_2(
		.Dq(m_Dq_2),
		.Addr(m_Addr_2),
		.Ba(m_Ba_2),
		.Clk(m_Clk_2),
		.Cke(m_Cke_2),
		.Cs_n(m_Cs_n_2),
		.Ras_n(m_Ras_n_2),
		.Cas_n(m_Cas_n_2),
		.We_n(m_We_n_2),
		.Dqm(m_Dqm_2)
	);




	bit request = 1'b0;
	bit rw = 1'b0;
	bit [31:0] address = 32'h0000_0000;
	bit [31:0] wdata = 32'hffff_ffff;
	// wire [31:0] rdata_1;
	wire [31:0] rdata_2;
	// wire ready_1;
	wire ready_2;



	// SDRAM_interface_2 sdram_controller_1(
	//     .i_reset(1'b0),
	//     .i_clock(clk),
	// 	.i_clock_sdram(clk_sdram),
	//     .i_request(request),
	//     .i_rw(rw),
	//     .i_address(address),
	//     .i_wdata(wdata),
	//     .o_rdata(rdata_1),
	//     .o_ready(ready_1),

	//     .sdram_clk(m_Clk_1),
	//     .sdram_clk_en(m_Cke_1),
	//     .sdram_cas_n(m_Cas_n_1),
	//     .sdram_ce_n(m_Cs_n_1),
	//     .sdram_ras_n(m_Ras_n_1),
	//     .sdram_we_n(m_We_n_1),
	//     .sdram_dqml(m_Dqm_1[0]),
	//     .sdram_dqmh(m_Dqm_1[1]),
	//     .sdram_ba(m_Ba_1),
	//     .sdram_addr(m_Addr_1),
	//     .sdram_data(m_Dq_1)
	// );



	logic [15:0] sdram_rdata;
	wire [15:0] sdram_wdata;
	wire sdram_data_rw;

    SDRAM_controller #(
        .FREQUENCY(100000000),
		.USER_DATA_WIDTH(32)
    ) sdram_controller_2(
	    .i_reset(1'b0),
	    .i_clock(clk),
		.i_clock_sdram(clk_sdram),
	    .i_request(request),
	    .i_rw(rw),
	    .i_address(address),
	    .i_wdata(wdata[31:0]),
	    .o_rdata(rdata_2),
	    .o_ready(ready_2),

	    .sdram_clk(m_Clk_2),
	    .sdram_clk_en(m_Cke_2),
	    .sdram_cas_n(m_Cas_n_2),
	    .sdram_cs_n(m_Cs_n_2),
	    .sdram_ras_n(m_Ras_n_2),
	    .sdram_we_n(m_We_n_2),
	    .sdram_dqm(m_Dqm_2),
	    .sdram_bs(m_Ba_2),		// Called BA in QMTech schematics
	    .sdram_addr(m_Addr_2),
		.sdram_rdata(sdram_rdata),
		.sdram_wdata(sdram_wdata),
		.sdram_data_rw(sdram_data_rw)
    );

	assign m_Dq_2 = sdram_data_rw ? sdram_wdata : 16'hz;
	assign sdram_rdata = m_Dq_2;







    initial begin
        clk = 0;
        #1;
        forever clk = #5 ~clk;
    end

	initial begin
		clk_sdram = 0;
		#9;
        forever clk_sdram = #5 ~clk_sdram;
    end

    initial begin
        $dumpfile("SDRAM_controlller_tb.vcd");
        $dumpvars(0, SDRAM_controller_tb);

		// init process
		repeat (100000) @ (posedge clk);

		// write
		rw <= 1'b1;
		address = 32'h00010000;
		wdata <= 32'hcafe_babe;
		request <= 1'b1;
		while (ready_2 != 1'b1) begin
			@ (posedge clk);
		end
		request <= 1'b0;
		$display("WRITTEN");

		while (ready_2 == 1'b1) begin
			@ (posedge clk);
		end

		// write
		rw <= 1'b1;
		address = 32'h01120ff4;
		wdata <= 32'hdead_beef;
		request <= 1'b1;
		while (ready_2 != 1'b1) begin
			@ (posedge clk);
		end
		request <= 1'b0;
		$display("WRITTEN");

		while (ready_2 == 1'b1) begin
			@ (posedge clk);
		end

		// read
		rw <= 1'b0;
		address = 32'h00010000;
		request <= 1'b1;
		while (ready_2 != 1'b1) begin
			@ (posedge clk);
		end
		request <= 1'b0;
		$display("READ %x - %x", rdata_2, rdata_2);

		while (ready_2 == 1'b1) begin
			@ (posedge clk);
		end

		repeat (10000) @ (posedge clk);

		// read
		rw <= 1'b0;
		address = 32'h01120ff4;
		request <= 1'b1;
		while (ready_2 != 1'b1) begin
			@ (posedge clk);
		end
		request <= 1'b0;
		$display("READ %x - %x", rdata_2, rdata_2);

		while (ready_2 == 1'b1) begin
			@ (posedge clk);
		end

		// idle
        repeat (10000) @ (posedge clk);

        $finish;
    end

endmodule
