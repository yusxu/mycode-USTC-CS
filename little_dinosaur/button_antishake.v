`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    11:04:58 12/11/2017 
// Design Name: 
// Module Name:    button_antishake 
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
module button_antishake(
	 input clk,
	 input in,
    output reg op
    );
reg [19:0]counter;
reg temp;
initial begin
	counter<=20'b0;
	temp<=1'b0;
end
always@(posedge clk)
begin
	if(in!=temp)
	begin 
		counter<=20'b0;
		temp<=in;
		op<=1'b0;
	end
	else if(counter==20'd1_000_000)
	begin
		temp<=in;
		if(in==1'b1) op<=1'b1;
		counter<=counter+1'b1;
	end
	else if(counter==20'd1_000_001) op<=1'b0;
	else if(in==temp) counter<=counter+1'b1;

end
endmodule