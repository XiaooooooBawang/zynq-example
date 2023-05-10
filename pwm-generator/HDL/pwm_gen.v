`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/04/16 23:04:32
// Design Name: 
// Module Name: pwm_gen
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


module pwm_gen #
    (parameter WIDTH=32) (
        input wire clk, rst,
        input wire [WIDTH-1:0] load, compare, control,
        output wire pwm_out
    );
    
    reg [WIDTH-1:0] count;
    reg pwm_out_r;
    
    // Make sure output is low if PWM is disabled
    assign pwm_out = control[0] & pwm_out_r;
    
    initial begin
        pwm_out_r = 1'b0;
        count = {WIDTH{1'b0}};
    end
    
    // The counter
    always @(posedge clk) begin
        if(!rst)
            count = {WIDTH{1'b0}};
        else if (count < load && control[0])
            count = count + 1;
        else
            count = 0;
    end
    
    always @(negedge clk) begin
        if(!rst)
            pwm_out_r = 1'b0;
        else begin
            case(count)
                0 : pwm_out_r = 1'b1;
                compare : pwm_out_r = 1'b0;
                default : pwm_out_r = pwm_out_r;
            endcase
        end
    end
endmodule
