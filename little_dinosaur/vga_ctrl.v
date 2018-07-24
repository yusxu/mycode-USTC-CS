`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:49:56 12/10/2017 
// Design Name: 
// Module Name:    vga_ctrl 
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
module vga_ctrl(
    input clk,
	 input [9:0]dino_y,
	 input [9:0]blk_x,
	 input rst,
	 input [21:0]spd,
	 input isover,
    output VGA_HS,
    output VGA_VS,
    output [2:0]VGA_R,
    output [2:0]VGA_G,
    output [1:0]VGA_B
    );

    reg [10:0] x_counter;
    reg [10:0] y_counter;
	 reg [29:0] timer;

    reg [7:0] RGBX;
	 reg [7:0] color;
	 
	 reg [9:0] dot_loc[13:0];
	 
	 reg [9:0]chrome;
	 
	 integer i;
	 integer j;
	 
    initial begin
        x_counter = 0;
        y_counter = 0;
		  tempclk = 0;
		  
		  RGBX = 0;
		  color = 0;
		  timer = 0;
		  
		  chrome = 700;
		  
		  for(i=0;i<14;i=i+1)begin
				dot_loc[i] = (i+2)*50;
		  end
		  cnt = 0;
    end

	 
	 reg tempclk;
	 always @(posedge clk)
		tempclk <= ~tempclk;
// Always block to drive drawing, {front|back}-doors, and syncs.
    always @(posedge tempclk) begin
        
		  if(x_counter == 1055)
        begin
            x_counter <= 0;

            if(y_counter == 627)
                y_counter <= 0;
            else
                y_counter <= y_counter + 1;
        end
        else
            x_counter <= x_counter + 1;

    end
	 
	 

    always @(x_counter or y_counter)
    begin
        /*if (x_counter<100)  GRBX<=3'b111;
            else if (x_counter<200)   GRBX<=3'b110;
            else if (x_counter<300)   GRBX<=3'b101;
            else if (x_counter<400)   GRBX<=3'b100;//green
            else if (x_counter<500)   GRBX<=3'b011;
            else if (x_counter<600)   GRBX<=3'b010;
            else if (x_counter<700)   GRBX<=3'b001;
            else  GRBX<=3'b000 ; */
			if(x_counter>800||y_counter>600) RGBX<=3'b000;
			else begin
				if((y_counter - 510)*(y_counter - 510) + (x_counter - dot_loc[0])*(x_counter - dot_loc[0]) <16 ) RGBX<=color;
				else if((y_counter - 520)*(y_counter - 520) + (x_counter - dot_loc[1])*(x_counter - dot_loc[1]) <16 ) RGBX<=color;
				else if((y_counter - 505)*(y_counter - 505) + (x_counter - dot_loc[2])*(x_counter - dot_loc[2]) <16 ) RGBX<=color;
				else if((y_counter - 525)*(y_counter - 525) + (x_counter - dot_loc[3])*(x_counter - dot_loc[3]) <16 ) RGBX<=color;
				else if((y_counter - 510)*(y_counter - 510) + (x_counter - dot_loc[4])*(x_counter - dot_loc[4]) <16 ) RGBX<=color;
				else if((y_counter - 520)*(y_counter - 520) + (x_counter - dot_loc[5])*(x_counter - dot_loc[5]) <16 ) RGBX<=color;
				else if((y_counter - 515)*(y_counter - 515) + (x_counter - dot_loc[6])*(x_counter - dot_loc[6]) <16 ) RGBX<=color;
				else if((y_counter - 520)*(y_counter - 520) + (x_counter - dot_loc[7])*(x_counter - dot_loc[7]) <16 ) RGBX<=color;
				//else if((y_counter - 505)*(y_counter - 505) + (x_counter - 450)*(x_counter - 450) <25 ) RGBX<=color;
				else if((y_counter - 515)*(y_counter - 515) + (x_counter - dot_loc[8])*(x_counter - dot_loc[8]) <16 ) RGBX<=color;
				else if((y_counter - 510)*(y_counter - 510) + (x_counter - dot_loc[9])*(x_counter - dot_loc[9]) <16 ) RGBX<=color;
				//else if((y_counter - 505)*(y_counter - 505) + (x_counter - 550)*(x_counter - 550) <25 ) RGBX<=color;
				else if((y_counter - 510)*(y_counter - 510) + (x_counter - dot_loc[10])*(x_counter - dot_loc[10]) <16 ) RGBX<=color;
				//else if((y_counter - 515)*(y_counter - 515) + (x_counter - 605)*(x_counter - 605) <25 ) RGBX<=color;
				else if((y_counter - 510)*(y_counter - 510) + (x_counter - dot_loc[11])*(x_counter - dot_loc[11]) <16 ) RGBX<=color;
				//else if((y_counter - 520)*(y_counter - 520) + (x_counter - 650)*(x_counter - 650) <25 ) RGBX<=color;
				else if((y_counter - 510)*(y_counter - 510) + (x_counter - dot_loc[12])*(x_counter - dot_loc[12]) <16 ) RGBX<=color;
				else if((y_counter - 505)*(y_counter - 505) + (x_counter - dot_loc[13])*(x_counter - dot_loc[13]) <16 ) RGBX<=color;
				
				else if(y_counter == 500) RGBX<=color;
				
				else begin
				
				/*if((x_counter-250)*(x_counter-250) + (y_counter-dino_y)*(y_counter-dino_y) < 50*50)
					RGBX<=color;
				else if((x_counter-blk_x)*(x_counter-blk_x)+(y_counter-475)*(y_counter-475) < 25*25)
							RGBX<=color;
				else RGBX<=~color;
				end*/
				
				//dino_y = 450;
				if(y_counter == dino_y-50 && x_counter>250 && x_counter <300) RGBX<=color;
				else if(x_counter == 300 && y_counter >dino_y-50 && y_counter<dino_y-20 ) RGBX<=color;
				else if(y_counter == dino_y-20 && x_counter >275 && x_counter <300) RGBX<=color;
				else if(y_counter == dino_y-30 && x_counter >285 && x_counter <300) RGBX<=color;
				else if((x_counter -280)*(x_counter -280)+(y_counter-dino_y+40)*(y_counter-dino_y+40)<4) RGBX<=color;
				else if(x_counter == 275 && y_counter >dino_y-20 && y_counter<dino_y+20 ) RGBX<=color;
				else if(y_counter == dino_y+20 && x_counter >200 && x_counter <275) RGBX<=color;
				else if(2*x_counter + 5*y_counter == 5*dino_y+500 && x_counter >235 && x_counter <250 ) RGBX<=color;
				else if(y_counter == dino_y+15 && x_counter >200 && x_counter <235) RGBX<=color;
				else if(x_counter == 200 && y_counter >dino_y+15 && y_counter <dino_y+20) RGBX<=color;
				else if(x_counter == 250 && y_counter >dino_y-50 && y_counter <dino_y+20) RGBX<=color;
				else if(x_counter == 255 && y_counter >dino_y+20 && y_counter <dino_y+50) RGBX<=color;
				else if(x_counter == 265 && y_counter >dino_y+20 && y_counter <dino_y+50) RGBX<=color;
				else if(y_counter == dino_y+5 && x_counter>270 && x_counter<285) RGBX<=color;
				else if(y_counter == dino_y-5 && x_counter>275 && x_counter<285) RGBX<=color;
				
				else begin 
						/*if((x_counter-blk_x)*(x_counter-blk_x)+(y_counter-475)*(y_counter-475) < 25*25)
							RGBX<=color;*/
					if(x_counter == blk_x -25 && y_counter >450 && y_counter <487) RGBX<=color;
					else if(x_counter == blk_x-18 && y_counter >450 && y_counter<480 ) RGBX<=color;
					else if(y_counter == 450 && x_counter>blk_x-25 && x_counter<blk_x-18) RGBX<=color;
					//else if((x_counter-20)*(x_counter-20)+(y_counter-430)*(y_counter-430)==) RGBX<=color;
					else if(y_counter == 480 && x_counter >blk_x-18 && x_counter <blk_x-15) RGBX<=color;
					else if(y_counter == 487 && x_counter >blk_x-25 && x_counter <blk_x-15) RGBX<=color;
					else if(x_counter == blk_x-15 && y_counter>450 &&y_counter <480) RGBX<=color;
					else if(x_counter == blk_x-15 && y_counter>487 && y_counter <500) RGBX<=color;
					else if(y_counter == 450 && x_counter >blk_x-15 && x_counter <blk_x+15) RGBX<=color;
					else if(x_counter == blk_x+15 && y_counter>450 && y_counter<460) RGBX<=color;
					else if(x_counter == blk_x+15 && y_counter >466 && y_counter <500) RGBX<=color;
					else if(x_counter == blk_x+25 && y_counter >450 && y_counter <466) RGBX<=color;
					else if(x_counter == blk_x+19 && y_counter >450 && y_counter <460) RGBX<=color;
					else if(y_counter == 450 && x_counter >blk_x+19 && x_counter <blk_x+25) RGBX<=color;
					else if(y_counter == 460 && x_counter >blk_x+15 && x_counter <blk_x+19) RGBX<=color;
					else if(y_counter == 466 && x_counter >blk_x+15 && x_counter <blk_x+25) RGBX<=color;
				
				
				
				/*else if(y_counter > 100 && y_counter < 140 && x_counter > chrome -50 && x_counter <chrome + 50) RGBX<=8'b111_000_00;
				else if(y_counter >140 && y_counter < 150 && x_counter >chrome-10 && x_counter<chrome+10 ) RGBX<=8'b000_000_11;
				else if(y_counter >140 && y_counter <200 && x_counter > chrome -50 && x_counter <chrome-10) RGBX<=8'b000_111_00;
				else if(y_counter >150 && y_counter <200 && x_counter >chrome -10 && x_counter<chrome) RGBX<=8'b000_111_00;
				else if(y_counter >140 && y_counter <200 && x_counter > chrome +10 && x_counter <chrome+50) RGBX<=8'b111_111_00;
				else if(y_counter >150 && y_counter <200 && x_counter >chrome  && x_counter<chrome+10) RGBX<=8'b111_111_00;*/
				
				else RGBX<=~color;
				end
				end
				
				
				
				
				/*if (x_counter>200 && x_counter<300 && y_counter<500 && y_counter>400)   GRBX<=3'b100;
				else if(x_counter>600 && x_counter<650 && y_counter<500 && y_counter>450) GRBX<=3'b001;
				else GRBX<=3'b111;


								if (x_counter>200 && x_counter<300 && y_counter<500 && y_counter>400)   GRBX<=3'b100;
								else if(x_counter>400 && x_counter<450 && y_counter<500 && y_counter>450) GRBX<=3'b001;
								else GRBX<=3'b111;
							end


							if(x_counter>800||y_counter>600) GRBX<=3'b000;
							else begin
							if (x_counter>200 && x_counter<300 && y_counter<400 && y_counter>300)   GRBX<=3'b100;
							else if(x_counter>200 && x_counter<250 && y_counter<500 && y_counter>450) GRBX<=3'b001;
							else GRBX<=3'b111;
							end


							if(x_counter>800||y_counter>600) GRBX<=3'b000;
							else begin
							if (x_counter>200 && x_counter<300 && y_counter<500 && y_counter>400)   GRBX<=3'b100;
							else if(x_counter>0 && x_counter<50 && y_counter<500 && y_counter>450) GRBX<=3'b001;
							else GRBX<=3'b111;
							end*/
			end

    end
	 
	 reg [21:0]cnt;
	 
	 always@(posedge clk or posedge rst) begin 
		if(rst) begin 
			color <= 0;
			for(i=0;i<14;i=i+1)begin
				dot_loc[i] <= (i+2)*50;
		  end
		  chrome <= 700;
		end
		else begin
		if(!isover) begin
		
		if(cnt == spd) begin
			for(j=0;j<14;j=j+1) begin
				cnt <= 22'b0;
				if(dot_loc[j] == 50) begin 
						dot_loc[j] <= 750;
			//if(speed >1_000_000) speed <= speed - 200_000;
				end
				else dot_loc[j] <= dot_loc[j] - 1;
			end
		end
		else cnt <= cnt +1;
		if(timer % 1_250_000 == 0)begin
			if(chrome == 50) chrome <= 700;
			else chrome <= chrome -1;
		end
		if(timer == 1_000_000_000) begin 
			color <= ~color;
			timer <=0;
			
		end
		else timer <= timer + 1;
		end
		
		end
	end

	
    assign  VGA_R=RGBX[7:5];
    assign  VGA_G=RGBX[4:2];
    assign  VGA_B=RGBX[1:0];

    assign VGA_HS = x_counter > 839 && x_counter < 968;
    assign VGA_VS = y_counter > 600 && y_counter < 605;

endmodule