`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:52:55 04/04/2018 
// Design Name: 
// Module Name:    regfile 
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
module regfile(
    input clk,
    input rst_n,
    input [4:0] ra_addr,
	 input [4:0] rb_addr,
	 input [4:0] w_addr,
    input [31:0] w_din,
    input w_en,
    output [31:0] ra,
	 output [31:0] rb
    );

reg [31:0] regs[0:31];

integer i;

initial begin 
	for(i=0;i<32;i=i+1) begin 
		regs[i] <= 0;
	end
end

always@(negedge clk,negedge rst_n)begin
	if(~rst_n) begin 
		for(i=0; i<32; i=i+1) begin 
			regs[i] <= 0;
		end
	end
	else begin
		if(w_en) regs[w_addr]<=w_din;
	end
end

assign ra = (ra_addr)? regs[ra_addr] : 0;
assign rb = (rb_addr)? regs[rb_addr] : 0;

endmodule
