module CPU_Fetch(
    input wire i_reset,
    input wire i_clock,

    // CPU
    output reg [31:0] o_pc,

    // Memory
    output reg o_request,
    output wire i_ready,
    output reg [31:0] o_address,
    input wire [31:0] i_data,

    // Decoder
    output reg [31:0] o_instruction,
    output reg o_fetched
);

    reg [2:0] state = 0;

    initial begin
        o_pc <= 0;
        o_request <= 0;
        o_address <= 0;
        o_instruction <= 0;
        o_fetched <= 0;
    end

    always @(posedge i_clock) begin
        
        case (state)
            0: begin
                $display("fetch %x", o_pc);
                o_address <= o_pc;
                o_request <= 1;
                state <= 1;
            end

            1: begin
                if (i_ready) begin

                    o_instruction <= i_data;
                    o_fetched <= 1;
                    state <= 2;

                end
            end

            2: begin
                o_fetched <= 0;
                o_pc <= o_pc + 4;
                state <= 0;
            end
        endcase

    end

endmodule
