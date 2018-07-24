`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:44:26 12/10/2017 
// Design Name: 
// Module Name:    statereg 
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
module statereg(
	input clk,
	input jump,
	input re_start,
	//input change_spd,
	input isover,
	
	input playernum,
	input showhighest,
	
	output [9:0]dinosaur_Y,
	output [9:0]block_X,
	
	output [21:0]spd,
	
	output [15:0]scr
    );
reg [15:0]now_score;
reg [15:0]highest[1:0];
reg [15:0]show_scr;	 
	 
reg [9:0] dino_y;
reg [9:0] blk_x;
reg [21:0] counter;
reg go_up;
reg doingjump;
//reg [21:0]speed;

//reg [19:0]dino_jump;

reg [21:0]speed[6:0];
integer i;


//reg [2:0]change_spd;
initial begin
	counter = 0;
	blk_x = 700;
	dino_y = 450;
	go_up = 1;
	doingjump = 0;
	//speed = 500_000;
	
	now_score = 0;
	highest[0] = 0;
	highest[1] = 0;
	
	
	i=0;
	speed[0] = 500_000;
	speed[1] = 400_000;
	speed[2] = 300_000;
	speed[3] = 200_000;
	speed[4] = 100_000;
	speed[5] = 600_000;
	speed[6] = 100_000;
	
end
always@(posedge clk or posedge re_start) begin
	
	if(re_start) begin 
		counter <= 0;
		blk_x <= 700;
		dino_y <= 450;
		go_up <= 1;
		doingjump <= 0;
		//speed <= 500_000;
		
		now_score <= 0;
		
		i <= (i+1)&7;
		
		/*speed[0] <= speed[1];
		speed[1] <= speed[2];
		speed[2] <= speed[3];
		speed[3] <= speed[4];
		speed[4] <= speed[5];
		speed[5] <= speed[6];
		speed[6] <= speed[0];*/
	end
	else if(!isover) begin
	//if(change_spd) speed <=speed - 200_000;
	if(jump) doingjump <= 1;
	if(counter == speed[i]) begin
		counter <= 22'b0;
		if(blk_x == 50) begin 
			blk_x <= 700;
			now_score <= now_score + 1'b1;
			
			i<=(i+1)%7;
			//if(speed >1_000_000) speed <= speed - 200_000;
		end
		else blk_x <= blk_x - 1;
	
	
		
	
		if(doingjump == 1) begin 
			if(go_up == 1 && dino_y<=325) go_up <= 0;
			else if(go_up == 1) dino_y <= dino_y - 1;
			else if(go_up == 0 && dino_y>=450) begin
				  doingjump <= 0;
				  go_up <= 1;
				  end
		   else dino_y <= dino_y + 1;
		end
		
	end
	else counter<=counter + 1;
	
				  
	
	end
	
	
	
	else if(highest[playernum] < now_score) begin 
			highest[playernum] <= now_score;
   end
		
		
		
end
/*always@(change_speed or re_start)begin 
	if(re_start) speed<=3333333;
	else begin
	if(change_speed ==3)begin 
		speed <=speed - 200_000;
		change_speed <= 0;
	end
	end
end*/
always@(posedge clk or posedge showhighest)begin 
	if(showhighest) begin
		show_scr <= highest[playernum];
	end
	else show_scr <= now_score;
end

assign scr = show_scr[15:0];

assign spd = speed[i];

assign dinosaur_Y=dino_y[9:0];
assign block_X=blk_x[9:0];
endmodule
