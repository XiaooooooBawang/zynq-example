#include "xparameters.h"
#include "PWM_Generator.h"
#include "xil_printf.h"
#include <stdio.h>

/*pwm generator的AXI总线基地址*/
#define XPAR_PWM_GENERATOR_0_S00_AXI_BASEADDR 0x43c00000

int main(void)
{
	init_platform();

	/* GPIO driver initialization */
	int status;
	status = XGpio_Initialize(&Gpio, GPIO_DEVICE_ID);
	if (status != XST_SUCCESS) {
		printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}


	// 20ms = 2000000 * 10ns
	int period = 2000000;

	PWM_GENERATOR_mDisableOutput(XPAR_PWM_GENERATOR_0_S00_AXI_BASEADDR);
	PWM_GENERATOR_mSetLoad(XPAR_PWM_GENERATOR_0_S00_AXI_BASEADDR, period);
    PWM_GENERATOR_mSetCompare(XPAR_PWM_GENERATOR_0_S_AXI_BASEADDR, 150000); 
    PWM_GENERATOR_mEnableOutput(XPAR_PWM_GENERATOR_0_S_AXI_BASEADDR);
	while(1)
	cleanup_platform();
	return 0;
}
