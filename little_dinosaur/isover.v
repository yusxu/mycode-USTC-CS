`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    23:14:50 12/11/2017 
// Design Name: 
// Module Name:    isover 
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
module isover(
	input [9:0]dinosaur_y,
	input [9:0]block_x,
	input restart,
	output isover
    );
reg over;
initial begin 
	over = 0;
end
always@(dinosaur_y or block_x or restart) begin
	if(restart) over <= 0;
	
	else begin 
	if((block_x - 250)*(block_x - 250)+(dinosaur_y - 475)*(dinosaur_y - 475)<50*50+25*25)begin
		over <= 1;
	end
	else over <= 0;
	end
end
assign isover = over;
endmodule
