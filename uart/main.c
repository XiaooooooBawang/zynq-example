/*
 * main.c
 *
 * Created on: May 4, 2023
 * Author: XiaooooooBawang
 */


#include "xparameters.h"
#include "platform.h"
#include "xuartps.h"
#include "sleep.h"
#include "xil_printf.h"
#include "stdio.h"
#include "xscugic.h"
#include <string.h>

#define BUFFER_SIZE 10
#define UART_Device_Id  XPAR_PS7_UART_1_DEVICE_ID  //�����豸ID
#define INTC_DEVICE_ID  XPAR_SCUGIC_SINGLE_DEVICE_ID  //�ж�ID
#define UART_INT_IRQ_ID     XPAR_XUARTPS_1_INTR  //�����ж�ID
#define INTC        XScuGic

XUartPs UartPs; /* Instance of the UART Device */
XUartPs_Config *Config;
INTC InterruptController;
u8 rec_data = 0;



u32 Uart1_init() {
	u32 Status;

	/* Initialize the UART driver so that it's ready to use */
	Config = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XUartPs_CfgInitialize(&UartPs, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	//���ô�����ֵ
	XUartPs_SetFifoThreshold(&UartPs, 1);

	/* Use normal mode */
	XUartPs_SetOperMode(&UartPs, XUARTPS_OPER_MODE_NORMAL);

	/* Set the baud rate */
	Status = XUartPs_SetBaudRate(&UartPs, 115200);
	if (Status != XST_SUCCESS) {
		printf("Set the baud rate failed\r\n");
		return XST_FAILURE;
	}

	/* Check hardware build */
	Status = XUartPs_SelfTest(&UartPs);
	if (Status != XST_SUCCESS) {
		printf("self test failed\r\n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

//�жϴ�������
void UART1_intr_handle(void *call_back_ref)
{
    XUartPs *uart_instance_ptr = (XUartPs *)call_back_ref;
//    u32 ReceivedCount = 0;
//    u8 RecvBuffer[BUFFER_SIZE] = {0}; /* Buffer for Receiving Data */

    u32 isr_status;
    //��ȡ�жϼĴ���
    isr_status = XUartPs_ReadReg(uart_instance_ptr->Config.BaseAddress,XUARTPS_IMR_OFFSET);
    isr_status &= XUartPs_ReadReg(uart_instance_ptr->Config.BaseAddress,XUARTPS_ISR_OFFSET);

    //�жϱ�־״̬λ���Ƿ�ΪRxFIFO����
    if(isr_status & XUARTPS_IXR_RXOVR)
    {
//    	ReceivedCount = XUartPs_Recv(&UartPs, RecvBuffer, BUFFER_SIZE);
    	rec_data = XUartPs_RecvByte(XPAR_PS7_UART_1_BASEADDR);
        XUartPs_WriteReg(uart_instance_ptr->Config.BaseAddress,XUARTPS_ISR_OFFSET,XUARTPS_IXR_RXOVR);//����ж�
    }

    /* ���ݴ��� */

    printf("recv_data : %d\r\n",rec_data);
}

//UART��������ʼ��
u32 uart_intr_init(INTC *IntcInstancePtr,XUartPs *UartInstancePtr,u16 UartIntrId)
{
    //UART�������жϳ�ʼ��
    XScuGic_Config *IntcConfig;
    IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
    XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
                        IntcConfig->CpuBaseAddress);
    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
                    (Xil_ExceptionHandler) XScuGic_InterruptHandler,
                    IntcInstancePtr);
    u32 status = XScuGic_Connect(IntcInstancePtr, UartIntrId,
                      (Xil_ExceptionHandler) UART1_intr_handle,
                      (void *) UartInstancePtr);
    if(status != XST_SUCCESS)
	{
		printf("Connect GIC failed\n");
		return XST_FAILURE;
	}
    XUartPs_SetInterruptMask(UartInstancePtr, XUARTPS_IXR_RXOVR);//����UART���жϴ�����ʽ
    XScuGic_Enable(IntcInstancePtr, UartIntrId);//ʹ���ж��豸
    Xil_ExceptionEnable();
    return XST_SUCCESS;
}


int main(){
	u32 status;
	u8 send_data=0;


	init_platform();

	/* initialize UART1 */
	status = Uart1_init();
	if(status != XST_SUCCESS) {
		printf("Uart1 initialization failed...\r\n");
		return 0;
	} else {
		printf("Successfully initialize Uart1.\r\n");
	}
	status = uart_intr_init(&InterruptController, &UartPs,UART_INT_IRQ_ID);
	if(status != XST_SUCCESS) {
		printf("uart_intr_init failed...\r\n");
		return 0;
	} else {
		printf("uart_intr_init success.\r\n");
	}

	while(1) {
		sleep(3);

//		u8 SendBuffer[BUFFER_SIZE] = "pynq"; /* Buffer for Transmitting Data */  // ���Զ����ַ���ĩβ��"\n"
//		XUartPs_Send(&UartPs, SendBuffer, BUFFER_SIZE);
		/* Wait until all of the data is sent */
//		while (XUartPs_IsSending(&UartPs)) {
//			/* Do nothing */
//		}


		send_data = 18;
		XUartPs_SendByte(XPAR_PS7_UART_1_BASEADDR,send_data);
		printf("send\r\n");
	}

	cleanup_platform();
}



