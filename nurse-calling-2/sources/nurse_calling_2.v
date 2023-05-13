`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: XDU
// Engineer: XiaooooooBawang
// 
// Create Date: 2023/05/09 15:19:38
// Design Name: 
// Module Name: nurse_calling_2
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


module nurse_calling_2
    #(
    parameter CNT_MAX = 21'd1999999 //计数器计数最大值
    )
    (
    input clk,
    input rst,
    input [2:0]call,
    output reg [2:0]led
    );
    
    reg [2:0]pos;
    reg [20:0]cnt_20ms ; //消抖计数器

    initial begin 
        led<=3'b000;
        pos<=3'd0;
    end 
    always @(posedge clk or posedge rst) begin
       if (rst)
        begin
            pos <= 3'd0;
            led <= 3'd0;
        end
      else if(cnt_20ms == CNT_MAX) begin 
        case(pos)
           3'b000:
            begin
               pos <= call;
               led <= pos; 
            end 
           3'b001,3'b010,3'b100:
            begin
                if(call < pos) begin
                    if(call > 0) begin
                        pos<=call;
                    end
                end
               led <= pos;
            end  
           default:
               pos <= 3'd0;
       endcase
       end
    end 
    
    //cnt_20ms:如果时钟的上升沿检测到外部按键输入的值为低电平时，计数器开始计数
    always@(posedge clk or posedge rst)
    if(rst)
        cnt_20ms <= 20'b0;
    else if(call > 1'b0)
        cnt_20ms <= 20'b0;
    else if(cnt_20ms == CNT_MAX && call == 1'b0)
        cnt_20ms <= cnt_20ms;
    else
        cnt_20ms <= cnt_20ms + 1'b1;
        
endmodule
