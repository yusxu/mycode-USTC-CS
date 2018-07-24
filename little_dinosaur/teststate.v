`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   21:25:59 12/10/2017
// Design Name:   statereg
// Module Name:   D:/Study/CS_Lab/Lab/little_dinosaur/teststate.v
// Project Name:  little_dinosaur
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: statereg
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module teststate;

	// Inputs
	reg clk;

	// Outputs
	wire [1:0] outstate;

	// Instantiate the Unit Under Test (UUT)
	statereg uut (
		.clk(clk), 
		.outstate(outstate)
	);

	initial begin
		// Initialize Inputs
		clk = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here
		/*clk=1;
		#10;
		clk=0;
		#10;
		clk=1;
		#10;
		clk=0;
		#10;
		clk=1;
		#10;
		clk=0;
		#10;
		clk=1;
		#10;
		clk=0;
		#10;
		clk=1;
		#10;
		clk=0;
		#10;
		clk=1;*/
	end
	always #10 clk=~clk;
      
endmodule

