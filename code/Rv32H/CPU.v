module CPU (
    i_clock,
    i_reset
);

input i_clock;
input i_reset;

reg [31:0] r_pc;

always @ (posedge i_clock)
begin
    if (reset == 1'b1) begin
        r_pc <= 0;
    end
    else begin

        // 1. Fetch instruction word.

    end
end

endmodule
