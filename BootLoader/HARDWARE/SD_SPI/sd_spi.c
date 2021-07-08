 /**
  ******************************************************************************
  * @file    bsp_spi_flash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   spi flash �ײ�Ӧ�ú���bsp 
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����STM32 F407 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
//#include "bsp_spi_flash.h"

#include "sd_spi.h"



 /**
  * @brief  SPI_FLASH��ʼ��
  * @param  ��
  * @retval ��
  */


#if  SET_SPI1
#if 1

#if 0
 void SPI1_Init(void)
 {	  
   GPIO_InitTypeDef  GPIO_InitStructure;
   SPI_InitTypeDef	SPI_InitStructure;
	 
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//��1?��GPIOB����?��
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//��1?��SPI1����?��
  
   //GPIOFB3,4,53?��??������??
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PB3~5?�䨮?1|?����?3?	 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//?�䨮?1|?��
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//��?������?3?
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//��?��-
   GPIO_Init(GPIOA, &GPIO_InitStructure);//3?��??��
	 
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PB3?�䨮??a SPI1
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PB4?�䨮??a SPI1
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PB5?�䨮??a SPI1
  
	 //?a��???????SPI?��3?��??��
	 RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//?��??SPI1
	 RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//����?1?��??SPI1
 
	 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����??SPI�̣�?��?��?????����?��y?Y?�꨺?:SPI����???a???????����???1��
	 SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		 //����??SPI1�����?�꨺?:����???a?��SPI
	 SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		 //����??SPI��?��y?Y�䨮D?:SPI����?��?����?8?????��11
	 SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	 //��?DD��?2?����?����????D���䨬??a??��???
	 SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	 //��?DD��?2?����?����?�̨�?t??��?��???�ꡧ��?��y?��???�̡�?��y?Y��?2��?��
	 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		 //NSSD?o?������2?t�ꡧNSS1��??��??1��?����?t�ꡧ��1��?SSI??��?1������:?��2?NSSD?o?��DSSI??????
	 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		 //?����?2����??��?�衤??�̦�??��:2����??��?�衤??��?��?a256
	 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //???����y?Y��?��?�䨮MSB???1��?LSB???a��?:��y?Y��?��?�䨮MSB???a��?
	 SPI_InitStructure.SPI_CRCPolynomial = 7;	 //CRC?��????��??��??��?
	 SPI_Init(SPI1, &SPI_InitStructure);  //?��?YSPI_InitStruct?D???����?2?��y3?��??����a����SPIx??��??��
  
	 SPI_Cmd(SPI1, ENABLE); //��1?��SPI��a����
 
	 SPI1_ReadWriteByte(0xff);//???����?��? 	  
 }	

#endif
 void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
 {
   assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//?D??��DD��D?
	 SPI1->CR1&=0XFFC7;//??3-5??��?��?��?���䨦��??2����??��
	 SPI1->CR1|=SPI_BaudRatePrescaler;	 //����??SPI1?��?�� 
	 SPI_Cmd(SPI1,ENABLE); //��1?��SPI1
 } 
 //SPI1 ?��D�䨰???��??��
 //TxData:��aD�䨨?��?��??��
 //����???��:?����?��?��?��??��
 u8 SPI1_ReadWriteByte(u8 TxData)
 {					  
  
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�̨���y����?��????  
	 
	 SPI_I2S_SendData(SPI1, TxData); //����1y��a����SPIx����?����???byte  ��y?Y
		 
   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�̨���y?����?������???byte  
  
	 return SPI_I2S_ReceiveData(SPI1); //����??����1ySPIx��??��?����?��?��y?Y	 
			 
 }
 
 
 #endif
 
 
 
void SD_SPI_Init_1(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ʹ�� FLASH_SPI ��GPIO ʱ�� */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO,SPI_FLASH_SPI_SCK_GPIO ʱ��ʹ�� */
  RCC_AHB1PeriphClockCmd (FLASH_SPI_SCK_GPIO_CLK_1 | FLASH_SPI_MISO_GPIO_CLK_1|FLASH_SPI_MOSI_GPIO_CLK_1|FLASH_CS_GPIO_CLK_1, ENABLE);

  /*!< SPI_FLASH_SPI ʱ��ʹ�� */
  FLASH_SPI_CLK_INIT_1(FLASH_SPI_CLK_1, ENABLE);
 
  //�������Ÿ���
  GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT_1,FLASH_SPI_SCK_PINSOURCE_1,FLASH_SPI_SCK_AF_1); 
	GPIO_PinAFConfig(FLASH_SPI_MISO_GPIO_PORT_1,FLASH_SPI_MISO_PINSOURCE_1,FLASH_SPI_MISO_AF_1); 
	GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT_1,FLASH_SPI_MOSI_PINSOURCE_1,FLASH_SPI_MOSI_AF_1); 
  
  /*!< ���� SPI_FLASH_SPI ����: SCK */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(FLASH_SPI_SCK_GPIO_PORT_1, &GPIO_InitStructure);
  
	/*!< ���� SPI_FLASH_SPI ����: MISO */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN_1;
  GPIO_Init(FLASH_SPI_MISO_GPIO_PORT_1, &GPIO_InitStructure);
  
	/*!< ���� SPI_FLASH_SPI ����: MOSI */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN_1;
  GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT_1, &GPIO_InitStructure);  

	/*!< ���� SPI_FLASH_SPI ����: CS */
  GPIO_InitStructure.GPIO_Pin = FLASH_CS_PIN_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(FLASH_CS_GPIO_PORT_1, &GPIO_InitStructure);

  /* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
  SPI_FLASH_CS_HIGH_1();

  /* FLASH_SPI ģʽ���� */
  // FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//SPI_NSS_Hard
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(FLASH_SPI_1, &SPI_InitStructure);

  /* ʹ�� FLASH_SPI_1  */
  SPI_Cmd(FLASH_SPI_1, ENABLE);

}

#elif SET_SPI3

#if 1

 void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
 {
   assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//?D??��DD��D?
	 SPI3->CR1&=0XFFC7;//??3-5??��?��?��?���䨦��??2����??��
	 SPI3->CR1|=SPI_BaudRatePrescaler;	 //����??SPI1?��?�� 
	 SPI_Cmd(SPI3,ENABLE); //��1?��SPI1
 } 
 //SPI1 ?��D�䨰???��??��
 //TxData:��aD�䨨?��?��??��
 //����???��:?����?��?��?��??��
 u8 SPI3_ReadWriteByte(u8 TxData)
 {					  
  
   while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET){}//�̨���y����?��????  
	 
	 SPI_I2S_SendData(SPI3, TxData); //����1y��a����SPIx����?����???byte  ��y?Y
		 
   while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET){} //�̨���y?����?������???byte  
  
	 return SPI_I2S_ReceiveData(SPI3); //����??����1ySPIx��??��?����?��?��y?Y	 
			 
 }
 
 
 #endif
 
 
 
void SD_SPI3_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ʹ�� FLASH_SPI ��GPIO ʱ�� */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO,SPI_FLASH_SPI_SCK_GPIO ʱ��ʹ�� */
  RCC_AHB1PeriphClockCmd (FLASH_SPI3_SCK_GPIO_CLK | FLASH_SPI3_MISO_GPIO_CLK|FLASH_SPI3_MOSI_GPIO_CLK|FLASH_CS_GPIO_CLK_3, ENABLE);

  /*!< SPI_FLASH_SPI ʱ��ʹ�� */
  FLASH_SPI3_CLK_INIT(FLASH_SPI3_CLK, ENABLE);
 
  //�������Ÿ���
  GPIO_PinAFConfig(FLASH_SPI3_SCK_GPIO_PORT,FLASH_SPI3_SCK_PINSOURCE,FLASH_SPI3_SCK_AF); 
	GPIO_PinAFConfig(FLASH_SPI3_MISO_GPIO_PORT,FLASH_SPI3_MISO_PINSOURCE,FLASH_SPI3_MISO_AF); 
	GPIO_PinAFConfig(FLASH_SPI3_MOSI_GPIO_PORT,FLASH_SPI3_MOSI_PINSOURCE,FLASH_SPI3_MOSI_AF); 
  
  /*!< ���� SPI_FLASH_SPI ����: SCK */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI3_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(FLASH_SPI3_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< ���� SPI_FLASH_SPI ����: MISO */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI3_MISO_PIN;
  GPIO_Init(FLASH_SPI3_MISO_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< ���� SPI_FLASH_SPI ����: MOSI */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI3_MOSI_PIN;
  GPIO_Init(FLASH_SPI3_MOSI_GPIO_PORT, &GPIO_InitStructure);  

	/*!< ���� SPI_FLASH_SPI ����: CS */
  GPIO_InitStructure.GPIO_Pin = FLASH_CS_PIN_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(FLASH_CS_GPIO_PORT_3, &GPIO_InitStructure);

  /* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
  SPI3_FLASH_CS_HIGH();
  //SPI_Cmd(SPI3,DISABLE);

  /* FLASH_SPI ģʽ���� */
  // FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(FLASH_SPI3, &SPI_InitStructure);

  /* ʹ�� FLASH_SPI_1  */
  SPI_Cmd(FLASH_SPI3, ENABLE);

}


#endif
