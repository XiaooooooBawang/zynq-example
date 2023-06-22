#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xiicps.h"
#include "vl6180_parameters.h"


#define IIC_DEV_ID 			XPAR_PS7_I2C_0_DEVICE_ID // IIC device ID
#define IIC_RATE 			100000  // standard mode is 100kbit/s
#define SLAVE_ADDR			VL6180_DEFAULT_I2C_ADDR // VL6180 7位设备地址 0x29
#define printf xil_printf


static 	XIicPs IicPs;  //IIC 实例
static 	XIicPs_Config * IicPs_Cfg;  //IIC 设备配置表（结构体）


// 初始化IIC，并设置IIC速率
int initIic()
{
	int status;
	// 1.查找外设的配置结构体
	IicPs_Cfg = XIicPs_LookupConfig(IIC_DEV_ID);
	if (NULL == IicPs_Cfg) {
		printf("Look up config failed \n");
		return XST_FAILURE;
	}
	// 2.初始化
	status = XIicPs_CfgInitialize(&IicPs, IicPs_Cfg, IicPs_Cfg->BaseAddress);
	if(status != XST_SUCCESS)
	{
		printf("initial IIC failed \n");
		return XST_FAILURE;
	}
	/*
	 * 设备自测 Perform a self-test to ensure that the hardware was built correctly.
	 */
	status = XIicPs_SelfTest(&IicPs);
	if (status != XST_SUCCESS) {
		printf("self test IIC failed \n");
		return XST_FAILURE;
	}
	//设置IIC速率
	status = XIicPs_SetSClk(&IicPs, IIC_RATE);
	if(status != XST_SUCCESS)
	{
		printf("set IIC clock rate failed \n");
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

s32 i2c_writeReg(XIicPs * iicPs, u16 slaveAddr, u8 * writeBuf)
{
	s32 status;
	//IIC写入寄存器地址，写入数据
	status = XIicPs_MasterSendPolled(iicPs, writeBuf, 3, SLAVE_ADDR);
	while (XIicPs_BusIsBusy(iicPs));  //等待接口写入完成
	if (status != XST_SUCCESS)
	{
		printf("write register failed! \n");
		return XST_FAILURE;
	}
	return status;
}

u8 i2c_readReg(XIicPs * iicPs, u16 slaveAddr, u8 * regAddr)
{
	s32 status ;
	u8 registerData;

	//发送设备地址，寄存器地址
	status = XIicPs_MasterSendPolled(iicPs, regAddr, 2, SLAVE_ADDR);
	while (XIicPs_BusIsBusy(iicPs));  //等待接口写入完成
	if (status != XST_SUCCESS)
	{
		printf("read register failed! \n");
		return XST_FAILURE;
	}

	//从寄存器中读出数据
	status = XIicPs_MasterRecvPolled(iicPs, &registerData, 1, SLAVE_ADDR);
	while (XIicPs_BusIsBusy(iicPs));  //等待接口写入完成
	if (status != XST_SUCCESS)
	{
		printf("read register failed! \n");
		return XST_FAILURE;
	}
	return registerData;
}

// 往16位地址的寄存器写8位的数据
s32 vl6180_write_reg(u16 regAddr, u8 data) {
	s32 status;
	u8 writeBuf[3] = {(regAddr >> 8), (regAddr & 0xff), data};
	status = i2c_writeReg(&IicPs, SLAVE_ADDR, writeBuf);
	if (status != XST_SUCCESS)
	{
		printf("vl6180 write register failed! \n");
		return XST_FAILURE;
	}
	return status;
}

// 读16位地址的寄存器中8位的数据
u8 vl6180_read_reg(u16 regAddr) {
	u8 reg_addr[2] = {(regAddr >> 8), (regAddr & 0xff)};
	return i2c_readReg(&IicPs, SLAVE_ADDR, reg_addr);
}

//u8 vl6180_read_id() {
//	return vl6180_read_reg(VL6180_REG_IDENTIFICATION_MODEL_ID);
//}

int vl6180_init()
{
//	u8 id = vl6180_read_id();
//	if(id == VL6180_DEFAULT_ID)
	if(1)
	{
		u16 config[39][2] = {
				{0x0207,0x01},
				{0x0208,0x01},
				{0x0096,0x00},
				{0x0097,0xfd},
				{0x00e3,0x00},
				{0x00e4,0x04},
				{0x00e5,0x02},
				{0x00e6,0x01},
				{0x00e7,0x03},
				{0x00f5,0x02},
				{0x00d9,0x05},
				{0x00db,0xce},
				{0x00dc,0x03},
				{0x00dd,0xf8},
				{0x009f,0x00},
				{0x00a3,0x3c},
				{0x00b7,0x00},
				{0x00bb,0x3c},
				{0x00b2,0x09},
				{0x00ca,0x09},
				{0x0198,0x01},
				{0x01b0,0x17},
				{0x01ad,0x00},
				{0x00ff,0x05},
				{0x0100,0x05},
				{0x0199,0x05},
				{0x01a6,0x1b},
				{0x01ac,0x3e},
				{0x01a7,0x1f},
				{0x0030,0x00},
				{0x0011,0x10},
				{0x010a,0x30},
				{0x003f,0x46},
				{0x0031,0xFF},
				{0x0040,0x63},
				{0x002e,0x01},
				{0x001b,0x09},
				{0x003e,0x31},
				{0x0014,0x24}
		};
		for (int i = 0; i < 31; i++) {
			vl6180_write_reg(config[i][0], config[i][1]);
		}

		// Recommended : Public registers - See data sheet for more detail
		vl6180_write_reg(config[31][0], config[31][1]);       // Enables polling for 'New Sample ready' when measurement completes
		vl6180_write_reg(config[32][0], config[32][1]);       // Set the averaging sample period(compromise between lower noise and increased execution time)
		vl6180_write_reg(config[33][0], config[33][1]);       // Sets the light and dark gain (uppernibble). Dark gain should not be changed. !上半字节要写入0x4	默认增益是1.0
		vl6180_write_reg(config[34][0], config[34][1]);       // sets the # of range measurements after which auto calibration of system is performed
		vl6180_write_reg(config[35][0], config[35][1]);       // Set ALS integration time to 100ms
		vl6180_write_reg(config[36][0], config[36][1]);       // perform a single temperature calibration of the ranging sensor

		// Optional: Public registers - See data sheet for more detail
		vl6180_write_reg(config[37][0], config[37][1]);    //测量间隔	轮询模式 period to 100ms	每步10ms->0-10ms
		vl6180_write_reg(config[38][0], config[38][1]);      //测量周期	ALS模式 to 500ms
		vl6180_write_reg(config[39][0], config[39][1]);       // Configures interrupt on 'New Sample Ready threshold event'
		return XST_SUCCESS;
	} else {
		return XST_FAILURE;
	}

}

u8 vl6180_read_range()
{
	u8 range = 0;
	//开启传输
	while(!(vl6180_read_reg(VL6180_REG_RESULT_RANGE_STATUS) & 0x01));
	//单次触发模式
	vl6180_write_reg(VL6180_REG_SYSRANGE_START, 0x01);
	//等待新样本就绪阈值事件(New Sample Ready threshold event)
	while(!(vl6180_read_reg(VL6180_REG_RESULT_INTERRUPT_STATUS_GPIO) & 0x04));
	//读距离数据
	range = vl6180_read_reg(VL6180_REG_RESULT_RANGE_VAL);
	//获取完数据，清楚中断位 0111b 清除了range、als、Error三种中断标志
	vl6180_write_reg(VL6180_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);
	return range;
}

int main()
{
    init_platform();

    //初始化IIC
    if(initIic() != XST_SUCCESS)
    {
    	printf("initialize IIC failed \r\n");
    	return XST_FAILURE;
    }

//    s32 status = vl6180_write_reg(VL6180_REG_IDENTIFICATION_MODEL_ID, 0xb4);
//    if (status != XST_SUCCESS)
//	{
//		printf("vl6180 write register failed! \n");
//		return XST_FAILURE;
//	}
//    u8 id = vl6180_read_id();
//    printf("id : %d", id);
    // 初始化vl6180
    if(vl6180_init() != XST_SUCCESS)
	{
		printf("initialize vl6180 failed \r\n");
		return XST_FAILURE;
	}

    // 读距离
    while (1) {
    	u8 range = vl6180_read_range();
    	printf("range : %d mm\r\n", range);
    	sleep(1);
    }

    cleanup_platform();
    return 0;
}
