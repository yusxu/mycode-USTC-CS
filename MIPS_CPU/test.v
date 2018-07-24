`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   22:21:25 05/31/2018
// Design Name:   top
// Module Name:   D:/Study/COD/Lab/MIPS_CPU/test.v
// Project Name:  MIPS_CPU
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: top
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test;

	// Inputs
	reg clk;
	reg rst_n;

	// Outputs
	wire [31:0] pc;
	wire [31:0] next_pc;
	wire [31:0] ins;
	wire [31:0] alu_a;
	wire [31:0] alu_b;
	wire [4:0] alu_op;
	wire [31:0] alu_out;
	wire [4:0] reg_w_addr;
	wire [31:0] reg_din;
	wire [31:0] forward_ra;
	wire [31:0] forward_rb;
	wire [7:0] mem_addr;
	wire [31:0] mem_din;
	wire [31:0] mem_dout;
	wire clean_n;
	wire stall;
	wire memtoreg;
	wire idex_memtoreg;

	// Instantiate the Unit Under Test (UUT)
	top uut (
		.clk(clk), 
		.rst_n(rst_n), 
		.pc(pc), 
		.next_pc(next_pc), 
		.ins(ins),
		.alu_a(alu_a),
		.alu_b(alu_b),
		.alu_op(alu_op), 
		.alu_out(alu_out), 
		.reg_w_addr(reg_w_addr), 
		.reg_din(reg_din), 
		.forward_ra(forward_ra), 
		.forward_rb(forward_rb), 
		.pos(pos),
		.exmem_branch(exmem_branch),
		.exmem_pos(exmem_pos),
		.mem_addr(mem_addr), 
		.mem_din(mem_din), 
		.mem_dout(mem_dout),
		.clean_n(clean_n),
		.stall(stall),
		.memtoreg(memtoreg),
		.idex_memtoreg(idex_memtoreg)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		rst_n = 1;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

	end
	
	always #5 clk = ~clk;
      
endmodule

