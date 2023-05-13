`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/05/09 17:09:52
// Design Name: 
// Module Name: s_nurse_calling_2
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


module s_nurse_calling_2;
    reg clk, rst;
    reg [2:0]call;
    wire [2:0]led;
    
    nurse_calling_2 dff(clk,rst,call,led);

    initial begin
        call <= 3'b000;
        clk<=0;
        rst<=0;
        #100     rst<=1;
        #5     rst<=0;
        #105   rst<=1;
        #5   rst<=0; 
    end
    always #5  clk <= ~clk;
    always #10 call <= call+1;
endmodule
