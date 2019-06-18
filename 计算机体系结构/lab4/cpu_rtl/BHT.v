`include "Parameters.v" 
module BHT(
    input wire clk, rst,
    input wire [31:0] PCD,PCF,PCE,BranchTargetD,
    input wire [2:0] BranchTypeD,
    input BranchE,
    output reg [31:0] NPCPred,
    output reg preBranchF
    );
    parameter BRANCH_ADDR_LEN = 4;
    parameter BRANCH_SIZE = 1<<BRANCH_ADDR_LEN;
    reg [31:0] BranchPC [BRANCH_SIZE-1:0];
    reg [31:0] PredictPC [BRANCH_SIZE-1:0];
    reg [1:0] valid [BRANCH_SIZE-1:0];

    wire [BRANCH_ADDR_LEN-1:0] pc_addr;
    wire [BRANCH_ADDR_LEN-1:0] new_pc_addr;
    wire [BRANCH_ADDR_LEN-1:0] result_pc_addr;
    assign result_pc_addr=PCE[BRANCH_ADDR_LEN+1:2];
    assign pc_addr=PCF[BRANCH_ADDR_LEN+1:2];
    assign new_pc_addr=PCD[BRANCH_ADDR_LEN+1:2];
    
    
    // IF hit or miss
    always @(*) begin
        if (PCF==BranchPC[pc_addr]) begin
            if (valid[pc_addr]==2'b11 || valid[pc_addr]==2'b10) begin
                NPCPred<=PredictPC[pc_addr];
                preBranchF<=1;
            end
            else begin
                NPCPred<=PCF+4;
                preBranchF<=0;
            end
        end
        else begin
            NPCPred<=PCF+4;
            preBranchF<=0;
        end      
    end
    // && BranchPC[new_pc_addr] != PCD
    // insert new record
    integer i;
    always@(posedge clk or posedge rst) begin
        if(rst) begin
            for(i=0;i<BRANCH_SIZE; i=i+1) begin
                BranchPC[i] <= 0;
                PredictPC[i] <= 0;
                valid[i] <= 2'b0;
            end
        end
        else begin
        if (BranchTypeD!=`NOBRANCH && BranchPC[new_pc_addr] != PCD) begin 
            BranchPC[new_pc_addr] <= PCD;
            PredictPC[new_pc_addr] <= BranchTargetD;
            valid[new_pc_addr]<= 2'b11;                    
        end
        end
    end

    // update
    always@(posedge clk) begin
        if (BranchPC[result_pc_addr]==PCE) begin
            if (BranchE ==1 && valid[result_pc_addr]!=2'b11) begin
                valid[result_pc_addr] <= valid[result_pc_addr] + 2'b01;
            end
            if (BranchE == 0 && valid[result_pc_addr]!=2'b00) begin
                valid[result_pc_addr] <= valid[result_pc_addr] - 2'b01;
            end
        end
    end
endmodule