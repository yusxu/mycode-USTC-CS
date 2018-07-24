`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:48:09 03/28/2018 
// Design Name: 
// Module Name:    ALU 
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
module ALU(
	input signed [31:0] alu_a,
	input signed [31:0] alu_b,
	input [4:0] alu_op,
	output reg signed [31:0] alu_out,
	output reg pos,
	output reg zero,
	output reg neg
    );

parameter A_NOP = 5'h00;
parameter A_ADD = 5'h01;
parameter A_SUB = 5'h02;
parameter A_AND = 5'h03;
parameter A_OR = 5'h04;
parameter A_XOR = 5'h05;
parameter A_NOR = 5'h06;
parameter LUI = 5'h07;
parameter SLT = 5'h08;
parameter DIV = 5'h09;
parameter MULT = 5'h0a;
parameter MUL = 5'h0b;
parameter MFHI = 5'h0c;
parameter MTHI = 5'h0d;
parameter MFLO = 5'h0e;
parameter MTLO = 5'h0f;
parameter MOVZ = 5'h10;
parameter MOVN = 5'h11;

reg [31:0] hi,lo;
wire [63:0] mult_result;
assign mult_result = alu_a * alu_b;

always@(*)begin
	case(alu_op[4:0])
		A_NOP: alu_out <= alu_out;
		A_ADD: alu_out <= alu_a + alu_b;
		A_SUB: alu_out <= alu_a - alu_b;
		A_AND: alu_out <= alu_a & alu_b;
		A_OR : alu_out <= alu_a | alu_b;
		A_XOR: alu_out <= alu_a ^ alu_b;
		A_NOR: alu_out <= ~(alu_a | alu_b);
		LUI: begin
			alu_out[31:16] <= alu_b[15:0];
			alu_out[15:0] <= 16'b0;
		end
		SLT: begin
			if(alu_a < alu_b) alu_out <= 1;
			else alu_out <= 0;
		end
		DIV: begin
			hi <= alu_a / alu_b;
			lo <= alu_a % alu_b;
		end
		MULT: begin
			hi <= mult_result[63:32];
			lo <= mult_result[31:0];
		end
		MUL: alu_out <= mult_result[31:0];
		MFHI: alu_out <= hi;
		MTHI: hi <= alu_a;
		MFLO: alu_out <= lo;
		MTLO: lo <= alu_a;
		MOVZ: begin
			if(alu_b == 0) alu_out <= alu_a;
			else alu_out <= 0;  // error 
		end
		MOVN: begin 
			if(alu_b != 0) alu_out <= alu_a;
			else alu_out <= 0; // error
		end
		default: alu_out <= alu_out;
	endcase
	
end

always@(*) begin
	if(alu_out > 0) begin
		pos <= 1;
		zero <= 0;
		neg <= 0;
	end
	
	else if(alu_out == 0) begin
		pos <= 0;
		zero <= 1;
		neg <= 0;
	end
	
	else if(alu_out < 0) begin
		pos <= 0;
		zero <= 0;
		neg <= 1;
	end
end

endmodule
