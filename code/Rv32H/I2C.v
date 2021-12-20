
module I2C #(
	parameter CLOCK_RATE = 50000000,
	parameter I2C_RATE = 20000
)(
	input wire i_clock,

	input wire i_enable,
	input wire [31:0] i_wdata,
	output reg o_ready,

	output wire I2C_SCL,
	inout wire I2C_SDA
);

	// Create internal I2C clock.
	wire i2c_clock;
	ClockDivider #(
		CLOCK_RATE,
		I2C_RATE
	) tx_clock(
		.i_clock(i_clock),
		.o_clock(i2c_clock)
	);


	reg [7:0] state = 255;
	reg [31:0] data;

	reg scl = 1;
	reg sda = 1;

	assign I2C_SCL = scl;
	assign I2C_SDA = sda ? 1'bz : 0;	// pulled up


	wire [7:0] address = data[23:16];
	wire [7:0] udata = data[15:8];
	wire [7:0] ldata = data[7:0];


	initial begin
		o_ready <= 0;
	end


	always @ (posedge i2c_clock) begin
	
		if (i_enable) begin
			if (state == 255) begin
				state <= 1;
				data <= i_wdata;
			end
		end
		else begin
			o_ready <= 0;
		end
	
		case (state)
			0: ;

			1: sda <= 0;
			2: scl <= 0;

			3: sda <= address[7];
			4: scl <= 1;
			5: scl <= 0;

			6: sda <= address[6];
			7: scl <= 1;
			8: scl <= 0;

			9: sda <= address[5];
			10: scl <= 1;
			11: scl <= 0;

			12: sda <= address[4];
			13: scl <= 1;
			14: scl <= 0;

			15: sda <= address[3];
			16: scl <= 1;
			17: scl <= 0;

			18: sda <= address[2];
			19: scl <= 1;
			20: scl <= 0;

			21: sda <= address[1];
			22: scl <= 1;
			23: scl <= 0;

			24: sda <= address[0];
			25: scl <= 1;
			26: scl <= 0;

			// write
			27: sda <= 0;
			28: scl <= 1;
			29: begin;
				scl <= 0;
				sda <= 1;	// put I2C_SDA high impedance, slave should pull low to ack.
			end

			// ack 1
			30: begin
				// \todo check I2C_SDA pulled low
				sda <= udata[7];
			end
			31: scl <= 1;
			32: scl <= 0;

			33: sda <= udata[6];
			34: scl <= 1;
			35: scl <= 0;

			36: sda <= udata[5];
			37: scl <= 1;
			38: scl <= 0;

			39: sda <= udata[4];
			40: scl <= 1;
			41: scl <= 0;

			42: sda <= udata[3];
			43: scl <= 1;
			44: scl <= 0;

			45: sda <= udata[2];
			46: scl <= 1;
			47: scl <= 0;

			48: sda <= udata[1];
			49: scl <= 1;
			50: scl <= 0;

			51: sda <= udata[0];
			52: scl <= 1;
			53: begin;
				scl <= 0;
				sda <= 1;	// put I2C_SDA high impedance, slave should pull low to ack.
			end

			// ack 2
			54: begin
				// \todo check I2C_SDA pulled low
				sda <= ldata[7];
			end
			55: scl <= 1;
			56: scl <= 0;

			57: sda <= ldata[6];
			58: scl <= 1;
			59: scl <= 0;

			60: sda <= ldata[5];
			61: scl <= 1;
			62: scl <= 0;

			63: sda <= ldata[4];
			64: scl <= 1;
			65: scl <= 0;

			66: sda <= ldata[3];
			67: scl <= 1;
			68: scl <= 0;

			69: sda <= ldata[2];
			70: scl <= 1;
			71: scl <= 0;

			72: sda <= ldata[1];
			73: scl <= 1;
			74: scl <= 0;

			75: sda <= ldata[0];
			76: scl <= 1;
			77: begin;
				scl <= 0;
				sda <= 1;	// put I2C_SDA high impedance, slave should pull low to ack.
			end

			// ack 3
			78: begin
				// \todo check I2C_SDA pulled low
				sda <= 0;
			end

			// stop
			79: scl <= 1;

			// ready
			80: o_ready <= 1;

		endcase

		if (state < 255)
			state <= state + 1;

	end

endmodule
