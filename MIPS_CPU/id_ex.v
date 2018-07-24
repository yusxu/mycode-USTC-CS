`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:45:54 05/29/2018 
// Design Name: 
// Module Name:    id_ex 
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
module id_ex(
	input clk,
	input rst_n,
	input clean_n,
	input stall,
	input breakpoint,
	
	input [31:0] ifid_npc,
	
	input Dmemtoreg,
	input Dmemwrite, 
	input [4:0] Dalu_op,
	input Dalu_srcB, 
	input Dregwrite,
	input Dregdst, 
	input [3:0] Dbranch,
	input Djump,
	input Djumpr,
	input [31:0] Dpc_jump,
	input Dtrap,
	
	input [31:0] Dra,
	input [31:0] Drb,
	
	input [31:0] Dsignimm,
	input [4:0] Drs,
	input [4:0] Drt,
	input [4:0] Drd,
	
	
	
	output reg [31:0] npc,
	
	output reg memtoreg,
	output reg memwrite, 
	output reg [4:0] alu_op,
	output reg alu_srcB, 
	output reg regwrite,
	output reg regdst, 
	output reg [3:0] branch,
	output reg jump,
	output reg jumpr,
	output reg [31:0] pc_jump,
	output reg trap,
	
	output reg [31:0] ra,
	output reg [31:0] rb,
	
	output reg [31:0] signimm,
	output reg [4:0] rs,
	output reg [4:0] rt,
	output reg [4:0] rd
    );

always@(posedge clk, negedge rst_n) begin
	if(~rst_n || ~clean_n || stall) begin
		npc <= 0;
		memtoreg <= 0;
	   memwrite <= 0;
		alu_op <= 0;
		alu_srcB <= 0; 
		regwrite <= 0;
		regdst <= 0; 
		branch <= 0;
		jump <= 0;
		jumpr <= 0;
	
		ra <= 0;
		rb <= 0;
	
		signimm <= 0;
		rs <= 0;
		rt <= 0;
		rd <= 0;
		pc_jump <= 0;
		trap <= 0;
	end
	
	else if(~breakpoint)begin
		npc <= ifid_npc;
		memtoreg <= Dmemtoreg;
	   memwrite <= Dmemwrite;
		alu_op <= Dalu_op;
		alu_srcB <= Dalu_srcB; 
		regwrite <= Dregwrite;
		regdst <= Dregdst; 
		branch <= Dbranch;
		jump <= Djump;
		jumpr <= Djumpr;
	
		ra <= Dra;
		rb <= Drb;
	
		signimm <= Dsignimm;
		rs <= Drs;
		rt <= Drt;
		rd <= Drd;
		pc_jump <= Dpc_jump;
		trap <= Dtrap;
	end
end


endmodule
