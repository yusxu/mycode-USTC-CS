`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:52:52 05/30/2018 
// Design Name: 
// Module Name:    mem_wb 
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
module mem_wb(
	input clk,
	input rst_n,
	input breakpoint,

	input exmem_memtoreg,
	input exmem_regwrite,
	input [31:0] mem_dout,
	input [31:0] exmem_alu_out,
	input [4:0] exmem_Dreg,
	
	output reg memwb_memtoreg,
	output reg memwb_regwrite,
	output reg [31:0] memwb_mdout,
	output reg [31:0] memwb_alu_out,
	output reg [4:0] memwb_Dreg
    );

always@(posedge clk, negedge rst_n) begin
	if(~rst_n)begin
		memwb_memtoreg <= 0;
		memwb_regwrite <= 0;
		memwb_mdout <= 0;
		memwb_alu_out <= 0;
		memwb_Dreg <= 0;
	end
	
	else if(~breakpoint)begin
		memwb_memtoreg <= exmem_memtoreg;
		memwb_regwrite <= exmem_regwrite;
		memwb_mdout <= mem_dout;
		memwb_alu_out <= exmem_alu_out;
		memwb_Dreg <= exmem_Dreg;
	end
end

endmodule
