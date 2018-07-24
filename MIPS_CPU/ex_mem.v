`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    11:09:58 05/30/2018 
// Design Name: 
// Module Name:    ex_mem 
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
module ex_mem(
	input clk,
	input rst_n,
	input clean_n,
	input breakpoint,
	
	input [31:0] npc_br,
	input idex_memtoreg,
	input idex_memwrite,
	input idex_regwrite,
	input [3:0] idex_branch,
	input idex_jump,
	input idex_jumpr,
	input [31:0] idex_pcjump,
	input [31:0] idex_pcjumpr,
	input idex_trap,
	
	input [31:0] exalu_out,
	input exalu_pos,
	input exalu_zero,
	input exalu_neg,
	
	input [31:0] idex_rb,
	input [4:0] exwritereg,
	
	output reg [31:0] npc,
	output reg memtoreg,
	output reg memwrite,
	output reg regwrite,
	output reg [3:0] branch,
	output reg jump,
	output reg jumpr,
	output reg [31:0] pc_jump,
	output reg [31:0] pc_jumpr,
	output reg trap,
	
	output reg [31:0] alu_out,
	output reg alu_pos,
	output reg alu_zero,
	output reg alu_neg,
	
	output reg [31:0] rb,
	output reg [4:0] writereg
    );

always@(posedge clk, negedge rst_n) begin
	if(~rst_n || ~clean_n) begin
		npc <= 0;
		memtoreg <= 0;
		memwrite <= 0;
		regwrite <= 0;
		branch <= 0;
		jump <= 0;
		jumpr <= 0;
		pc_jump <= 0;
		pc_jumpr <= 0;
		trap <= 0;
	
		alu_out <= 0;
		alu_pos <= 0;
		alu_zero <= 0;
		alu_neg <= 0;
	
		rb <= 0;
		writereg <= 0;
	end
	else if(~breakpoint)begin
		npc <= npc_br;
		memtoreg <= idex_memtoreg;
		memwrite <= idex_memwrite;
		regwrite <= idex_regwrite;
		branch <= idex_branch;
		jump <= idex_jump;
		jumpr <= idex_jumpr;
		pc_jump <= idex_pcjump;
		pc_jumpr <= idex_pcjumpr;
		trap <= idex_trap;
	
		alu_out <= exalu_out;
		alu_pos <= exalu_pos;
		alu_zero <= exalu_zero;
		alu_neg <= exalu_neg;
	
		rb <= idex_rb;
		writereg <= exwritereg;
	end
end
endmodule
