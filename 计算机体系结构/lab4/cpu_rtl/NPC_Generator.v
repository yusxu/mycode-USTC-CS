`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB（Embeded System Lab）
// Engineer: Haojun Xia
// Create Date: 2019/03/14 11:21:33
// Design Name: RISCV-Pipline CPU
// Module Name: NPC_Generator
// Target Devices: Nexys4
// Tool Versions: Vivado 2017.4.1
// Description: Choose Next PC value
//////////////////////////////////////////////////////////////////////////////////
module NPC_Generator(
    input wire [31:0] PCE,JalrTarget, BranchTarget, JalTarget, NPCPred,
    input wire BranchE,JalD,JalrE, preBranchE,
    output reg [31:0] PC_In
    );
    always @(*)
    begin
        if(JalrE)
            PC_In <= JalrTarget;
        else if(BranchE == 1 && preBranchE == 0)
            PC_In <= BranchTarget;
        else if(BranchE == 0 && preBranchE == 1)
            PC_In <= PCE + 4;
        else if(JalD)
            PC_In <= JalTarget;
        else
            PC_In <= NPCPred;
    end
endmodule
