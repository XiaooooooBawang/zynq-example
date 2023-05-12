`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: XDU
// Engineer: XiaooooooBawang
// 
// Create Date: 2023/05/09 14:33:32
// Design Name: 
// Module Name: nurse_calling
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module nurse_calling(
//    input clk,
    input [3:0]call,
    output reg [3:0]led
    );
    initial begin
       led=4'b0000;
    end
    
    always@(*)
        casex(call)
        4'b0000:
            led<=4'b0000;
        4'bxxx1:
            led<=4'b0001;//0
        4'bxx10:
            led<=4'b0010;//1
        4'bx100:
            led<=4'b0100;//2
        4'b1000:
            led<=4'b1000;//3
    endcase
    
//    ila_0 ila_inst(
//        .clk(clk),
//        .probe0(call),
//        .probe1(led)
//    );
endmodule

