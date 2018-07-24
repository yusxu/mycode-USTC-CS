`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:02:39 05/28/2018 
// Design Name: 
// Module Name:    control 
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
module control(
	input [5:0] op,
	input [5:0] funct,
	output reg memtoreg,
	output reg memwrite,
	
	output reg [4:0] alu_op,
	//output reg alu_srcA,
	output reg alu_srcB,
	
	output reg regwrite,
	output reg regdst,
	
	output reg [3:0] branch,
	output reg jump,
	output reg jumpr,
	output reg breakpoint,
	output reg trap
    );

initial begin
	breakpoint <= 0;
end

parameter bgtz = 1;
parameter bne = 2;
parameter beq = 3; //+
parameter blez = 4; //+
parameter bltz = 5; //+
//parameter bgez = 6; //000001 rs 00001 ++

//i+
//beq blez bltz slt sltu slti sltiu addiu ori xori 
//lui div divu mult multu mul mov mfhi mflo mthi
//mtlo movn movz breakpoint teq teqi tge tgeu beqz bnez

always@(*) begin 
	case(op[5:0])
	0: begin  // R-type
		case(funct[5:0])
			6'b100000: begin	// add
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 1;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'b100001: begin // addu
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 1; alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'b100010: begin  // sub
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 2;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'b100011: begin // subu
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0;
				regwrite <= 1; alu_op <= 2;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'b100100: begin // and
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0;
				regwrite <= 1; alu_op <= 3;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'b100101: begin // or
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 4; alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'b100110: begin // xor
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 5; alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'b100111: begin // nor
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 6;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'b001000: begin // jr
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 0;  alu_srcB <= 0; regdst <= 0; jumpr <= 1;
			end
			6'b101010: begin // slt+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 8;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'b101011: begin // sltu+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 8;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'ha: begin // movz+  not complete
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 16;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'hb: begin // movn+ not complete
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 17;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'hd: begin // breakpoint+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 0;  alu_srcB <= 0; regdst <= 0; jumpr <= 0;
				breakpoint <= 1; //else <=0 not done
			end
			6'h10: begin // mfhi+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 12;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'h11: begin // mthi+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 13;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'h12: begin // mflo+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 14;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'h13: begin // mtlo+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 15;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'h1a: begin // div+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 9;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'h1b: begin // divu+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 9;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'h18: begin // mult+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 10;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'h19: begin // multu+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 10;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
			end
			6'h30: begin // tge+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 2;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
				trap <= 1;
			end
			6'h31: begin // tgeu+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 2;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
				trap <= 1;
			end
			6'h34: begin // teq+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 2;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
				trap <= 1; //others trap <= 0
			end
			6'h36: begin // tne+
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 2;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
				trap <= 1;
			end
			
			/*6'b000000: begin //sll +
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 9;  alu_srcB <= 0; regdst <= 0; jumpr <= 0;
			end
			6'b000100: begin //sllv +
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 9; alu_srcB <= 0; regdst <= 0; jumpr <= 0;
			end
			6'b000010: begin // srl +
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 5'h10;  alu_srcB <= 0; regdst <= 0; jumpr <= 0;
			end
			6'b000110: begin // srlv +
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 1; alu_op <= 5'h10; alu_srcB <= 0; regdst <= 0; jumpr <= 0;
			end*/
			default: begin
				memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
				regwrite <= 0; alu_op <= 0;  alu_srcB <= 0; regdst <= 0; jumpr <= 0;
			end
			endcase
	end
	8: begin    /*addi*/
		memtoreg <= 0; 
		memwrite <= 0;
		branch <= 0;
		jump <= 0;
		regwrite <= 1;
		alu_op <= 1;    //+
		alu_srcB <= 1;    //sign imm
		regdst <= 0;    //rt
		jumpr <= 0;
	end
	9: begin // addiu+
		memtoreg <= 0; 
		memwrite <= 0;
		branch <= 0;
		jump <= 0;
		regwrite <= 1;
		alu_op <= 1;    //+
		alu_srcB <= 1;    //sign imm
		regdst <= 0;    //rt
		jumpr <= 0;
	end
	6'ha: begin // slti+
		memtoreg <= 0; 
		memwrite <= 0;
		branch <= 0;
		jump <= 0;
		regwrite <= 1;
		alu_op <= 8;    
		alu_srcB <= 1;    //sign imm
		regdst <= 0;    //rt
		jumpr <= 0;
	end
	6'hb: begin // sltiu+
		memtoreg <= 0; 
		memwrite <= 0;
		branch <= 0;
		jump <= 0;
		regwrite <= 1;
		alu_op <= 8;    
		alu_srcB <= 1;    //sign imm
		regdst <= 0;    //rt
		jumpr <= 0;
	end
	6'hc: begin  // andi
		memtoreg <= 0; 
		memwrite <= 0;
		branch <= 0;
		jump <= 0;
		regwrite <= 1;
		alu_op <= 3;    
		alu_srcB <= 1;    //sign imm
		regdst <= 0;    //rt
		jumpr <= 0;
	end
	6'hd: begin // ori+  imm15 zero extend! not complete
		memtoreg <= 0; 
		memwrite <= 0;
		branch <= 0;
		jump <= 0;
		regwrite <= 1;
		alu_op <= 4;    
		alu_srcB <= 1;    //sign imm
		regdst <= 0;    //rt
		jumpr <= 0;
	end
	6'he: begin //xori+  imm15 zero extend!
		memtoreg <= 0; 
		memwrite <= 0;
		branch <= 0;
		jump <= 0;
		regwrite <= 1;
		alu_op <= 5;    
		alu_srcB <= 1;    //sign imm
		regdst <= 0;    //rt
		jumpr <= 0;
	end
	6'hf: begin  //lui+
		memtoreg <= 0; 
		memwrite <= 0;
		branch <= 0;
		jump <= 0;
		regwrite <= 1;
		alu_op <= 7;    
		alu_srcB <= 1;    //sign imm
		regdst <= 0;    //rt
		jumpr <= 0;
	end
	6'h1c: begin // mul+
		memtoreg <= 0; memwrite <= 0; branch <= 0; jump <= 0; 
		regwrite <= 1; alu_op <= 11;  alu_srcB <= 0; regdst <= 1; jumpr <= 0;
	end
	6'b100_011: begin   /*lw*/               
		memtoreg <= 1;
		memwrite <= 0;
		branch <= 0;
		jump <= 0;
		alu_op <= 1;    //+
		alu_srcB <= 1;    //sign imm
		regdst <= 0;    //rt
		regwrite <= 1;
		jumpr <= 0;
	end
	6'b101_011: begin   /*sw*/
		memtoreg <= 0;
		memwrite <= 1;
		branch <= 0;
		alu_op <= 1;
		alu_srcB <= 1;   //sign imm
		regdst <= 0;
		regwrite <= 0;
		jump <= 0;
		jumpr <= 0;
	end
	2: begin   /*jump*/
		memtoreg <= 0;
		memwrite <= 0;
		branch <= 0;
		alu_op <= 1;
		alu_srcB <= 0;   
		regdst <= 0;
		regwrite <= 0;
		jump <= 1;
		jumpr <= 0;
	end
	/*3: begin // jal+
		memtoreg <= 0;
		memwrite <= 0;
		branch <= 0;
		alu_op <= 1;
		alu_srcB <= 0;   
		regdst <= 0;
		regwrite <= 1;
		jump <= 0;
		jumpr <= 0;
		jal <= 1;
	end*/
	7: begin  /*bgtz*/
		memtoreg <= 0;
		memwrite <= 0;
		branch <= bgtz;
		alu_op <= 2;
		alu_srcB <= 0;   // rb
		regdst <= 0;
		regwrite <= 0;
		jump <= 0;
		jumpr <= 0;
	end
	5: begin  // bne
		memtoreg <= 0;
		memwrite <= 0;
		branch <= bne;
		alu_op <= 2;
		alu_srcB <= 0;   // rb
		regdst <= 0;
		regwrite <= 0;
		jump <= 0;
		jumpr <= 0;
	end
	4: begin //beq +
		memtoreg <= 0;
		memwrite <= 0;
		branch <= beq;
		alu_op <= 2;
		alu_srcB <= 0;   // rb
		regdst <= 0;
		regwrite <= 0;
		jump <= 0;
		jumpr <= 0;
	end
	6: begin //blez+
		memtoreg <= 0;
		memwrite <= 0;
		branch <= blez;
		alu_op <= 2;
		alu_srcB <= 0;   // rb
		regdst <= 0;
		regwrite <= 0;
		jump <= 0;
		jumpr <= 0;
	end
	1: begin //bltz+
		memtoreg <= 0;
		memwrite <= 0;
		branch <= bltz;
		alu_op <= 2;
		alu_srcB <= 0;   // rb
		regdst <= 0;
		regwrite <= 0;
		jump <= 0;
		jumpr <= 0;
	end
	
	default: begin  //nop
		memtoreg <= 0;
		memwrite <= 0;
		branch <= 0;
		alu_op <= 0;
		alu_srcB <= 0;   
		regdst <= 0;
		regwrite <= 0;
		jump <= 0;
		jumpr <= 0;
	end
	endcase
end

endmodule