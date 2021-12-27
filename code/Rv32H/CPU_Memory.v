module CPU_Memory(
    input wire i_reset,
    input wire i_clock,
    input wire i_execute,

    // Bus
    output reg o_bus_rw,
    output reg o_bus_request,
    output wire i_bus_ready,
    output reg [31:0] o_bus_address,
    input wire [31:0] i_bus_rdata,
    output reg [31:0] o_bus_wdata,

    // Input
    input wire [4:0] i_inst_rd,
    input wire [31:0] i_rd,
    input wire i_branch,
    input wire [31:0] i_pc_next,
    input wire i_mem_read,
    input wire i_mem_write,
    input wire [31:0] i_mem_address,

    // Output
    output reg [4:0] o_inst_rd,
    output reg [31:0] o_rd,
    output reg o_branch,
    output reg [31:0] o_pc_next,
    output reg o_ready
);

    reg read_pending = 0;
    reg write_pending = 0;

    initial begin
        o_bus_rw <= 0;
        o_bus_request <= 0;
        o_bus_address <= 0;
        o_bus_wdata <= 0;
        o_inst_rd <= 0;
        o_rd <= 0;
        o_branch <= 0;
        o_pc_next <= 0;
        o_ready <= 0;
    end

    always @(posedge i_clock) begin

        if (read_pending) begin
            if (i_bus_ready) begin
                o_bus_request <= 0;
                o_rd <= i_bus_rdata;
                o_ready <= 1;
                read_pending <= 0;
            end
        end

        if (write_pending) begin
            if (i_bus_ready) begin
                o_bus_request <= 0;
                o_ready <= 1;
                write_pending <= 0;
            end
        end

        if (i_execute) begin
            // Pass through
            o_inst_rd <= i_inst_rd;
            o_rd <= i_rd;
            o_branch <= i_branch;
            o_pc_next <= i_pc_next;

            if (i_mem_read) begin
                $display("memory read %x begin", i_mem_address);
                o_bus_rw <= 0;
                o_bus_request <= 1;
                o_bus_address <= i_mem_address;
                read_pending <= 1;
            end
            else if (i_mem_write) begin
                $display("memory write %x begin", i_mem_address);
                o_bus_rw <= 1;
                o_bus_request <= 1;
                o_bus_address <= i_mem_address;
                o_bus_wdata <= i_rd;
                write_pending <= 1;
            end
            else begin
                // No memory access, immediately ready.
                o_ready <= 1;
            end
        end
        else begin
            o_inst_rd <= 0;
            o_ready <= 0;
        end

    end

endmodule
