#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xiicps.h"
#include "TCS34725_parameters.h"
#include <stdio.h>


#define IIC_DEV_ID 			XPAR_PS7_I2C_0_DEVICE_ID // IIC device ID
#define IIC_RATE 			400000  // fast mode is 400kbit/s
#define SLAVE_ADDR			TCS34725_ADDRESS // TCS34725 7λ�豸��ַ 0x29


static 	XIicPs IicPs;  //IIC ʵ��
static 	XIicPs_Config * IicPs_Cfg;  //IIC �豸���ñ��ṹ�壩


// ��ʼ��IIC��������IIC����
int initIic()
{
	int status;
	// 1.������������ýṹ��
	IicPs_Cfg = XIicPs_LookupConfig(IIC_DEV_ID);
	if (NULL == IicPs_Cfg) {
		printf("Look up config failed \n");
		return XST_FAILURE;
	}
	// 2.��ʼ��
	status = XIicPs_CfgInitialize(&IicPs, IicPs_Cfg, IicPs_Cfg->BaseAddress);
	if(status != XST_SUCCESS)
	{
		printf("initial IIC failed \n");
		return XST_FAILURE;
	}
	/*
	 * �豸�Բ� Perform a self-test to ensure that the hardware was built correctly.
	 */
	status = XIicPs_SelfTest(&IicPs);
	if (status != XST_SUCCESS) {
		printf("self test IIC failed \n");
		return XST_FAILURE;
	}
	//����IIC����
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
	//IICд��8λ��������(0x100xxxxx��xxxxxΪ5λ�Ĵ�����ַ)��д��8λ����
	status = XIicPs_MasterSendPolled(iicPs, writeBuf, 2, SLAVE_ADDR);
	while (XIicPs_BusIsBusy(iicPs));  //�ȴ��ӿ�д�����
	if (status != XST_SUCCESS)
	{
		printf("write register failed! \n");
		return XST_FAILURE;
	}
	return status;
}

u8 i2c_readReg(XIicPs * iicPs, u16 slaveAddr, u8 * regAddr)
{
	s32 status;
	u8 registerData;

	//д��8λ��������(0x100xxxxx��xxxxxΪ5λ�Ĵ�����ַ)
	status = XIicPs_MasterSendPolled(iicPs, regAddr, 1, SLAVE_ADDR);
	while (XIicPs_BusIsBusy(iicPs));  //�ȴ��ӿ�д�����
	if (status != XST_SUCCESS)
	{
		printf("read register failed! \n");
		return XST_FAILURE;
	}
	//�ӼĴ����ж���8λ����
	status = XIicPs_MasterRecvPolled(iicPs, &registerData, 1, SLAVE_ADDR);
	while (XIicPs_BusIsBusy(iicPs));  //�ȴ��ӿ�д�����
	if (status != XST_SUCCESS)
	{
		printf("read register failed! \n");
		return XST_FAILURE;
	}
	return registerData;
}

u16 i2c_readReg16(XIicPs * iicPs, u16 slaveAddr, u8 * regAddr)
{
	s32 status;
	u8 registerData[2] = {0,0};

	//д��8λ��������(0x100xxxxx��xxxxxΪ5λ�Ĵ�����ַ)
	status = XIicPs_MasterSendPolled(iicPs, regAddr, 1, SLAVE_ADDR);
	while (XIicPs_BusIsBusy(iicPs));  //�ȴ��ӿ�д�����
	if (status != XST_SUCCESS)
	{
		printf("read register failed! \n");
		return XST_FAILURE;
	}
	//�ӼĴ����ж���8λ����
	status = XIicPs_MasterRecvPolled(iicPs, registerData, 2, SLAVE_ADDR);
	while (XIicPs_BusIsBusy(iicPs));  //�ȴ��ӿ�д�����
	if (status != XST_SUCCESS)
	{
		printf("read register failed! \n");
		return XST_FAILURE;
	}
	return ((u16)(registerData[1]) << 8) | ((u16)(registerData[0]) & 0xFF);
}

// ��8λ��ַ�ļĴ���д8λ�����ݣ���Ҫд������ֵ���ɣ�
s32 TCS34725_write_reg(u8 regAddr, u8 data) {
	s32 status;
	u8 writeBuf[2] = {(u8)(TCS34725_COMMAND_BIT | regAddr), data};
	status = i2c_writeReg(&IicPs, SLAVE_ADDR, writeBuf);
	if (status != XST_SUCCESS)
	{
		printf("TCS34725 write register failed! \n");
		return XST_FAILURE;
	}
	return status;
}

// ��8λ��ַ�ļĴ�����8λ������
u8 TCS34725_read_reg(u8 regAddr) {
	u8 reg_addr = (u8)(TCS34725_COMMAND_BIT | regAddr);
	return i2c_readReg(&IicPs, SLAVE_ADDR, &reg_addr);
}

// ��8λ��ַ�ļĴ�����16λ������
u16 TCS34725_read_reg16(u8 regAddr) {
	u8 reg_addr = (u8)(TCS34725_COMMAND_BIT | regAddr);
	return i2c_readReg16(&IicPs, SLAVE_ADDR, &reg_addr);
}

// ��TCS34725���豸id��Ӧ��Ϊ0x44
u8 TCS34725_read_id() {
	return TCS34725_read_reg(TCS34725_ID);
}

// TCS34725ʹ��
void TCS34725_enable(void)
{
	TCS34725_write_reg(TCS34725_ENABLE, TCS34725_ENABLE_PON);
	TCS34725_write_reg(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
}

// TCS34725ʧ��
void TCS34725_disable(void)
{
	u8 cmd = TCS34725_read_reg(TCS34725_ENABLE);
	cmd = cmd & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
	TCS34725_write_reg(TCS34725_ENABLE, cmd);
}

// TCS34725 ��ʼ��
s32 TCS34725_init()
{
	u8 id = TCS34725_read_id();
	if(id == TCS34725_DEVICE_ID)
	{
		TCS34725_write_reg(TCS34725_ATIME, TCS34725_INTEGRATIONTIME_50MS);
		TCS34725_write_reg(TCS34725_CONTROL, TCS34725_GAIN_1X);
		TCS34725_enable();
		return XST_SUCCESS;
	} else {
		return XST_FAILURE;
	}

}

// ��ȡ16λ����ɫͨ������
u16 TCS34725_GetChannelData(u8 reg) {
	return TCS34725_read_reg16(reg);
}

u8 TCS34725_getRawData(u16 *r, u16 *g, u16 *b, u16 *c) {
	u8 status = TCS34725_STATUS_AVALID;

	status = TCS34725_read_reg(TCS34725_STATUS);

	if(status & TCS34725_STATUS_AVALID)
	{
		*c = TCS34725_GetChannelData(TCS34725_CDATAL);
		*r = TCS34725_GetChannelData(TCS34725_RDATAL);
		*g = TCS34725_GetChannelData(TCS34725_GDATAL);
		*b = TCS34725_GetChannelData(TCS34725_BDATAL);
		return 1;
	}
	return 0;
}

void TCS34725_getRGB(float *r, float *g, float *b) {
  u16 red, green, blue, clear;
  u8 status = TCS34725_getRawData(&red, &green, &blue, &clear);
  u32 sum = clear;

  // Avoid divide by zero errors ... if clear = 0 return black
  // ������δ������Ҳ���ɶ�
  if (clear == 0 || !status) {
    *r = *g = *b = 0;
    return;
  }

  float x = (float)red / sum * 255.0;
  float y = (float)green / sum * 255.0;
  float z = (float)blue / sum * 255.0;

  *r = x;
  *g = y;
  *b = z;
}

int main()
{
    init_platform();

    //��ʼ��IIC
    if(initIic() != XST_SUCCESS)
    {
    	printf("initialize IIC failed \r\n");
    	return XST_FAILURE;
    }

    // ��ʼ��TCS347256180
    if(TCS34725_init() != XST_SUCCESS)
	{
		printf("initialize TCS34725 failed \r\n");
		return XST_FAILURE;
	}

    float r = 0, g = 0, b = 0;
    // ��rgbc����
    while (1) {
    	TCS34725_getRGB(&r, &g, &b);
    	printf("r : %.0f g : %.0f b : %.0f \r\n", r, g, b);
    	sleep(1);
    }

    cleanup_platform();
    return 0;
}
