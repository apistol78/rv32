/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

typedef struct packed
{
	logic [31:0] x;
	logic [31:0] y;
}
vec2_t;

module GPU_Triangle(
    input i_clock,

    input i_request,
    output logic o_ready,

    input vec2_t i_min,
    input vec2_t i_max,

    input vec2_t i_v0,
    input vec2_t i_v1,
    input vec2_t i_v2,

    output logic [9:0] o_fb_x,
    output logic [9:0] o_fb_y,
    output logic o_fb_wr
);

    logic [2:0] state = 0;

    logic signed [31:0] a01;
    logic signed [31:0] b01;
    logic signed [31:0] a12;
    logic signed [31:0] b12;
    logic signed [31:0] a20;
    logic signed [31:0] b20;

    logic signed [31:0] w0_row;
    logic signed [31:0] w1_row;
    logic signed [31:0] w2_row;

    logic [31:0] x;
    logic [31:0] y;
    logic signed [31:0] w0;
    logic signed [31:0] w1;
    logic signed [31:0] w2;

    initial begin
        o_ready = 0;
        o_fb_wr = 0;
    end

    function [31:0] orient2d(
        input vec2_t a,
        input vec2_t b,
        input vec2_t c
    );
    begin
        orient2d = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    end
    endfunction

    always @(posedge i_clock) begin

        o_ready <= 0;

        case (state)
        0: begin
            // Wait until all parameters are ready.
            if (i_request)
                state <= 1;
        end

        // Calculate setup
        1: begin
            a01 <= i_v0.y - i_v1.y;
            b01 <= i_v1.x - i_v0.x;
            a12 <= i_v1.y - i_v2.y;
            b12 <= i_v2.x - i_v1.x;
            a20 <= i_v2.y - i_v0.y;
            b20 <= i_v0.x - i_v2.x;
            state <= 2;
        end

        // Calculate w0_row, w1_row, w2_row
        2: begin
            w0_row <= orient2d(i_v1, i_v2, i_min);
            w1_row <= orient2d(i_v2, i_v0, i_min);
            w2_row <= orient2d(i_v0, i_v1, i_min);
            state <= 3;
        end

        3: begin
            y <= i_min.y;
            x <= i_min.x;
            w0 <= w0_row;
            w1 <= w1_row;
            w2 <= w2_row;
            state <= 4;
        end

        4: begin
            o_fb_wr <= 0;
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) begin
                o_fb_x <= x;
                o_fb_y <= y;
                o_fb_wr <= 1;
            end

            w0 <= w0 + a12;
            w1 <= w1 + a20;
            w2 <= w2 + a01;
     
            x <= x + 1;
            if (x >= i_max.x) begin
                x <= i_min.x;
                y <= y + 1;
                if (y >= i_max.y) begin
                    o_ready <= 1;
                    state <= 0;
                end
            end
        end

        endcase
    end

endmodule
