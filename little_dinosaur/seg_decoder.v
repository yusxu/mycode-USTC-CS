`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:41:55 12/11/2017 
// Design Name: 
// Module Name:    seg_decoder 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module seg_decoder(
	 input clk,
    input [15:0]num,
	 output reg [3:0]segsel,
    output reg [6:0]data
    );
reg [19:0]counter;
reg [3:0]show;
initial begin 
	counter = 0;
end
always@(posedge clk) begin
	counter<=counter+1;
	if(counter==20'd100_000) begin
		case(segsel[3:0])
		~4'b0001:begin segsel<=~4'b0010; show<=num[7:4]; end
		~4'b0010:begin segsel<=~4'b0100; show<=num[11:8]; end
		~4'b0100:begin segsel<=~4'b1000; show<=num[15:12]; end
		~4'b1000:begin segsel<=~4'b0001; show<=num[3:0]; end
		default:begin segsel<=~4'b0001; show<=num[3:0]; end
		endcase
		counter<=20'b0;
	end
end
always @(*) 
begin
	case(show[3:0])
	4'b0000:data<=~7'b1111110;
	4'b0001:data<=~7'b0110000;
	4'b0010:data<=~7'b1101101;
	4'b0011:data<=~7'b1111001;
	4'b0100:data<=~7'b0110011;
	4'b0101:data<=~7'b1011011;
	4'b0110:data<=~7'b1011111;
	4'b0111:data<=~7'b1110000;
	4'b1000:data<=~7'b1111111;
	4'b1001:data<=~7'b1111011;
	4'b1010:data<=~7'b1110111;
	4'b1011:data<=~7'b0011111;
	4'b1100:data<=~7'b1001110;
	4'b1101:data<=~7'b0111101;
	4'b1110:data<=~7'b1001111;
	4'b1111:data<=~7'b1000111;
	default:data<=~7'b0000000;
	endcase
end

endmodule
