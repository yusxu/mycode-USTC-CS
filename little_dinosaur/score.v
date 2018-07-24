`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:27:14 12/11/2017 
// Design Name: 
// Module Name:    score 
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
module score(
	input [9:0]block_x,
	//input [3:0]playernum,
	input showhighest,
	input isover,
	input rst,
	output [15:0]scr
	//output reg change_speed
    );
//reg [15:0] players_score[3:0];
//reg [1:0]counter;
reg [15:0]now_score;
reg [15:0]highest;
reg [15:0]show_scr;
initial begin
	//counter = 0;
	now_score = 0;
	highest = 0;
end
// 1A35 3A75
always@(block_x or isover or rst) begin
	if(rst) now_score <= 0;
	else begin
	if(isover)begin 
		if(highest < now_score) begin 
			highest <= now_score;
		end
		/*else begin 
			highest <= highest;
			now_score <= now_score;
		end*/
	end
	else begin
	
	if(block_x == 200) begin
		now_score <= now_score + 1;
		//counter <= counter + 1;
		//if(counter == 3) begin
			//change_speed <=1;
			//counter <= 0;
		//end
	end
	//else now_score <= now_score;
	
	//else change_speed <= 0;
	end
	
	end
end

always@(showhighest)begin 
	if(showhighest) begin
		show_scr <= highest;
	end
	else show_scr <= now_score;
end
assign scr = show_scr[15:0];
endmodule
