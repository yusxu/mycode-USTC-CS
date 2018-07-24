`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:48:33 04/25/2018 
// Design Name: 
// Module Name:    PC 
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
module pc(
	input clk,
	input rst_n,
	input breakpoint,
	input pcwrite,
	input [31:0] next_pc,
	output reg [31:0] pc
    );

initial begin
	pc <= 0;
end

always@(posedge clk, negedge rst_n) begin
	if(~rst_n) 
		pc <= 0;
	else
		if(pcwrite == 1 && ~breakpoint)
			pc <= next_pc;
end

endmodule
