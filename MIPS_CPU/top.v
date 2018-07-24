`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:23:52 04/25/2018 
// Design Name: 
// Module Name:    top 
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
module top(
	input clk,
	input rst_n,
	
	output [31:0] pc,
	output reg [31:0] next_pc,
	
	output [31:0] ins,
	
	output [31:0] sign_imm32,
	output [31:0] left2_imm,
	output [31:0] idex_imm,
	
	output [31:0] alu_a,
	output [31:0] alu_b,
	output [4:0] alu_op,
	output [31:0] alu_out,
	
	output [4:0] reg_w_addr,
	output [31:0] reg_din,
	
	output [31:0] forward_ra,
	output [31:0] forward_rb,
	output pos,
	output [3:0] exmem_branch,
	output exmem_pos,
	
	output [7:0] mem_addr,
	output [31:0] mem_din,
	output [31:0] mem_dout,
	
	output reg clean_n,
	output reg stall,
	output memtoreg,
	output idex_memtoreg
    );

//reg clean_n;
//reg stall;
initial begin
	clean_n <= 1;
	stall <= 0;
end

wire exmem_regwrite,memwb_regwrite;
wire [4:0] exmem_Dreg,memwb_Dreg;

parameter plus4 = 0;
parameter br = 1;
parameter jmp = 2;
parameter jmpr = 3;
parameter trp = 4;
reg [2:0] pcsrc;


/*IF*/
wire [31:0] pc, pc_plus4,ins_out;
wire pcwrite;
assign pcwrite = (~stall) || (exmem_branch != 0) || exmem_jump || exmem_jumpr ;
//reg [31:0] next_pc;
assign pc_plus4 = pc + 4;
always@(*) begin
	case(pcsrc[2:0])
		plus4: next_pc <= pc_plus4;
		br: next_pc <= exmem_npc;
		jmp: next_pc <= exmem_pcjump;
		jmpr: next_pc <= exmem_pcjumpr;
		trp: next_pc <= 32'hffff;
		default: next_pc <= pc_plus4;
	endcase
end

/*IF/ID*/
wire [31:0] ifid_npc, ins; 


/*ID*/
wire memtoreg,memwrite,alu_srcB,regwrite,regdst,jump,jumpr;
wire [3:0] branch;
wire [31:0] sign_imm32,ra,rb,pc_jump;
wire [4:0] Dalu_op;
wire breakpoint,trap;
//jump
assign pc_jump[27:2] = ins[25:0];
assign pc_jump[1:0]  = 2'b0;
assign pc_jump[31:28]= ifid_npc[31:28]; 

//stall pipeline
always@(*) begin
	if(idex_memtoreg == 1 && (idex_rt == ins[25:21] || idex_rt == ins[20:16]))
		stall <= 1;
	else stall <= 0;
end

/*ID/EX*/
wire [31:0] idex_npc;
wire idex_memtoreg,idex_memwrite,idex_alu_srcB, idex_regwrite,idex_regdst, idex_jump,idex_jumpr,idex_trap;
wire [3:0] idex_branch;
wire [4:0] idex_alu_op;
wire [31:0] idex_ra,idex_rb,idex_imm,idex_pcjump;
wire [4:0] idex_rs,idex_rt,idex_rd;


/*EX*/
//PC
wire [31:0] left2_imm, pc_br,pc_jumpr;  
wire [4:0] alu_op;
assign left2_imm[31:2] = idex_imm[29:0];
assign left2_imm[1:0]  = 2'b0;
assign pc_br = idex_npc + left2_imm;
assign pc_jumpr = idex_ra;

//ALU
wire [31:0] alu_a,alu_b,alu_out;
reg [31:0] forward_ra,forward_rb;
always@(*) begin
	case(forwardA[1:0])
		0: forward_ra <= idex_ra;
		1: forward_ra <= reg_din;
		2: forward_ra <= exmem_alu_out;
		default: forward_ra <= idex_ra;
	endcase
	
	case(forwardB[1:0])
		0: forward_rb <= idex_rb;
		1: forward_rb <= reg_din;
		2: forward_rb <= exmem_alu_out;
		default: forward_rb <= idex_rb;
	endcase
end

assign alu_a = forward_ra;
assign alu_b = (idex_alu_srcB)? idex_imm : forward_rb;
assign alu_op = idex_alu_op;
//rt rd
wire [4:0] Dreg;
assign Dreg = (idex_regdst)? idex_rd : idex_rt;

//forwarding
wire [1:0] forwardA,forwardB;
forward fwd(
	idex_rs,idex_rt,exmem_regwrite,exmem_Dreg,memwb_regwrite,memwb_Dreg,
	forwardA,forwardB
);


/*EX/MEM*/
wire [31:0] exmem_npc,exmem_alu_out,exmem_rb,exmem_pcjump,exmem_pcjumpr;
wire exmem_memtoreg,exmem_memwrite,exmem_jump,exmem_jumpr,exmem_trap;
wire [3:0] exmem_branch;
wire exmem_pos,exmem_zero,exmem_neg;
//wire [4:0] exmem_Dreg;


/*MEM*/
//mem
wire [7:0] mem_addr;
wire [31:0] mem_din,mem_dout;
assign mem_addr[7:0] = exmem_alu_out[9:2];
assign mem_din = exmem_rb;
//branch jump jumpr

//reset pipeline reg before mem
always@(*)begin
	if(exmem_branch == 1 && exmem_pos == 1) begin
		pcsrc <= br;  //bgtz
		clean_n <= 0;
	end
	else if(exmem_branch == 2 && ~exmem_zero) begin
		pcsrc <= br;  //bne
		clean_n <= 0;
	end
	else if(exmem_branch == 3 && exmem_zero) begin
		pcsrc <= br; //beq
		clean_n <= 0;
	end
	else if(exmem_branch == 4 && ~exmem_pos) begin
		pcsrc <= br; //blez
		clean_n <= 0;
	end
	else if(exmem_branch == 5 && exmem_neg) begin
		pcsrc <= br; //bltz
		clean_n <= 0;
	end
	else if(exmem_jump) begin
		pcsrc <= jmp; //jump
		clean_n <= 0;
	end
	else if(exmem_jumpr)begin
		pcsrc <= jmpr; //jumpr
		clean_n <= 0;
	end
	else if(exmem_trap && exmem_zero) begin //teq
		pcsrc <= trp;
		clean_n <=0;
	end
	else if(exmem_trap && ~exmem_zero) begin //tne 
		pcsrc <= trp;
		clean_n <=0;
	end
	else if(exmem_trap && ~exmem_neg) begin //tge tgeu
		pcsrc <= trp;
		clean_n <=0;
	end
	else begin
		pcsrc <= plus4; //pc+4
		clean_n <= 1;
	end
	
end

/*MEM/WB*/
wire memwb_memtoreg;
wire [31:0] memwb_mdout,memwb_alu_out;
//wire [4:0] memwb_Dreg;


/*WB*/
wire [31:0] reg_din;
wire [4:0] reg_w_addr;
assign reg_din = (memwb_memtoreg)? memwb_mdout : memwb_alu_out;
assign reg_w_addr = memwb_Dreg;

pc p(clk, rst_n,breakpoint, pcwrite, next_pc, pc);

i_mem i(pc[9:2],ins_out);

if_id ifid(clk, rst_n,clean_n,stall,breakpoint, pc_plus4, ins_out, ifid_npc, ins);

control c(ins[31:26],ins[5:0], memtoreg,memwrite, Dalu_op,alu_srcB, regwrite,regdst, branch,jump,jumpr,breakpoint,trap);

regfile rf(clk,rst_n,ins[25:21],ins[20:16],reg_w_addr,reg_din,memwb_regwrite,ra,rb);

signext ex(ins[15:0],sign_imm32);

id_ex idex(
	clk,rst_n,clean_n,stall,breakpoint,
	ifid_npc, 
	memtoreg,memwrite, Dalu_op,alu_srcB, 
	regwrite,regdst, branch,jump,jumpr, pc_jump, trap,
	ra,rb,sign_imm32,
	ins[25:21],ins[20:16],ins[15:11],
	idex_npc, idex_memtoreg,idex_memwrite,
	idex_alu_op,idex_alu_srcB, idex_regwrite,idex_regdst, 
	idex_branch,idex_jump,idex_jumpr,idex_pcjump, idex_trap,
	idex_ra,idex_rb,idex_imm,
	idex_rs,idex_rt,idex_rd
);

ALU alu(alu_a,alu_b,alu_op,alu_out,pos,zero,neg);

ex_mem exmem(
	clk,rst_n,clean_n,breakpoint,pc_br,
	idex_memtoreg,idex_memwrite,idex_regwrite, 
	idex_branch,idex_jump,idex_jumpr,idex_pcjump,pc_jumpr, idex_trap,
	alu_out,pos,zero,neg,forward_rb,Dreg,
	exmem_npc,exmem_memtoreg,exmem_memwrite,exmem_regwrite, 
	exmem_branch,exmem_jump,exmem_jumpr,exmem_pcjump,exmem_pcjumpr, exmem_trap,
	exmem_alu_out,exmem_pos,exmem_zero,exmem_neg,
	exmem_rb,exmem_Dreg
);

d_mem d(mem_addr,mem_din,clk,exmem_memwrite,mem_dout);

mem_wb memwb(
	clk,rst_n,breakpoint,exmem_memtoreg,exmem_regwrite,mem_dout,exmem_alu_out,exmem_Dreg,
	memwb_memtoreg,memwb_regwrite,memwb_mdout,memwb_alu_out,memwb_Dreg
);

endmodule
