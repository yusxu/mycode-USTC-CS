`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:47:54 05/31/2018 
// Design Name: 
// Module Name:    forward 
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
module forward(
	input [4:0] idex_rs,
	input [4:0] idex_rt,
	input exmem_regwrite,
	input [4:0] exmem_rd,
	input memwb_regwrite,
	input [4:0] memwb_rd,
	
	output reg [1:0] forwardA,
	output reg [1:0] forwardB
    );

always@(*) begin
	if(exmem_regwrite == 1 && exmem_rd !=0 && exmem_rd == idex_rs)
		forwardA <= 2;
	else if(memwb_regwrite == 1 && memwb_rd != 0 && memwb_rd == idex_rs)
		forwardA <= 1;
	else 
		forwardA <= 0;
	
	if(exmem_regwrite == 1 && exmem_rd !=0 && exmem_rd == idex_rt)
		forwardB <= 2;
	else if(memwb_regwrite == 1 && memwb_rd != 0 && memwb_rd == idex_rt)
		forwardB <= 1;
	else 
		forwardB <= 0;
end

endmodule
