`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:39:52 05/29/2018 
// Design Name: 
// Module Name:    if_id 
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
module if_id(
	input clk,
	input rst_n,
	input clean_n,
	input stall,
	input breakpoint,
	input [31:0] pc_plus4,
	input [31:0] ins_out,
	output reg [31:0] npc,
	output reg [31:0] ins
    );

always@(posedge clk, negedge rst_n) begin

	if(~rst_n || ~clean_n) begin
		npc <= 0;
		ins <= 0;
	end
	
	else if(~stall && ~breakpoint)begin
		npc <= pc_plus4;
		ins <= ins_out;
	end
end


endmodule
