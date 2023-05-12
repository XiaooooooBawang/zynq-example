`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/05/09 19:30:26
// Design Name: 
// Module Name: s_nurse_calling
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

module s_nurse_calling;
//    reg [3:0]clk;
    reg [3:0]call;
    wire [3:0]led;
    
    nurse_calling dff(clk,call,led);

    initial begin
        clk <= 0;
//        call <= 4'b0011;
        call <=4'b0000;
    end
//    always #15 call[0] <= !call[0];
//    always #5 clk<=!clk;
    always #10 call <= call+1;
endmodule
