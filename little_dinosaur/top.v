`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:45:19 12/10/2017 
// Design Name: 
// Module Name:    top 
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
module top(
	input clk,
	input Jump,
	input restart,
	//input [3:0]player_num,
	input player_num,
	input show_highest,
	output [3:0]seg_sel,
	output [6:0]seg_data,
	output VGA_HS,
   output VGA_VS,
   output [2:0]VGA_R,
   output [2:0]VGA_G,
   output [1:0]VGA_B
    );
wire [9:0]interdino;
wire [9:0]interblk;
wire [15:0]inter_num;
wire [21:0]inter_spd;

//statereg statereg(clk,op,restart,change_spd,inter_isover,interdino,interblk);
statereg statereg(clk,op,restart,inter_isover,player_num,show_highest,interdino,interblk,inter_spd,inter_num);

vga_ctrl vga_ctrl(clk,interdino,interblk,restart,inter_spd,inter_isover,VGA_HS,VGA_VS,VGA_R,VGA_G,VGA_B);

button_antishake button_antishake(clk,Jump,op);

//score score(interblk,player_num,show_highest,inter_isover,inter_num,change_spd);
//score score(interblk,player_num,show_highest,inter_isover,inter_num);
//score score(interblk,show_highest,inter_isover,restart,inter_num);

isover isover(interdino,interblk,restart,inter_isover);

seg_decoder seg(clk,inter_num,seg_sel,seg_data);

endmodule
