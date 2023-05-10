#include "xparameters.h"
#include "PWM_Generator.h"

/*pwm generator AXI base address*/
#define XPAR_PWM_GENERATOR_0_S00_AXI_BASEADDR 0x43c00000

int main(void)
{
	/* 
        The default clock period for zynq is 10ns.
        In this example, my servo is controlled by pwm, 
        and the period of this servo is 20ms, so I set prd as 2000000.
        20ms = 2000000 * 10ns
     */
	int prd = 2000000;

	PWM_GENERATOR_mDisableOutput(XPAR_PWM_GENERATOR_0_S00_AXI_BASEADDR);
	PWM_GENERATOR_mSetLoad(XPAR_PWM_GENERATOR_0_S00_AXI_BASEADDR, prd);
    PWM_GENERATOR_mSetCompare(XPAR_PWM_GENERATOR_0_S00_AXI_BASEADDR, 150000); // set the duty cycle
    PWM_GENERATOR_mEnableOutput(XPAR_PWM_GENERATOR_0_S00_AXI_BASEADDR);
	while(1)
	
	return 0;
}
