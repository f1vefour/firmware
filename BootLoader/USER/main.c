#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "malloc.h"    
#include "ff.h"  
#include "exfuns.h"    
#include "stmflash.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mmc_sd.h"
#include "sd_spi.h"

 
#define VERSIONADDR 0x08010000
#define SIZE		sizeof("SMPRINTERV0101.bin")
#define PATHUP		"0:/UPDATE/"

#define FLASH_APP1_ADDR		0x08011000  	//��һ��Ӧ�ó������ʼ��ַ
#define UPDATEADDR 			0x08010000		//����

typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���
iapfun jump2app; 
u32 iapbuf[512];

//appxaddr:Ӧ�ó������ʼ��ַ
//appbuf:Ӧ�ó���CODE.
//appsize:Ӧ�ó����С(�ֽ�).
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u32 t;
	u16 i=0;
	u32 temp;
	u32 fwaddr=appxaddr;//��ǰд��ĵ�ַ
	u8 *dfu=appbuf;
	for(t=0;t<appsize;t+=4)
	{						   
		temp=(u32)dfu[3]<<24;   
		temp|=(u32)dfu[2]<<16;    
		temp|=(u32)dfu[1]<<8;
		temp|=(u32)dfu[0];	  
		dfu+=4;//ƫ��4���ֽ�
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0; 
			STMFLASH_Write(fwaddr,iapbuf,512);
			fwaddr+=2048;//ƫ��2048   512*4=2048
		}
	} 
	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ.  
}


//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();									//��ת��APP.
	}
}	

void montor_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_9|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_SetBits(GPIOE,GPIO_Pin_6|GPIO_Pin_9|GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_3|GPIO_Pin_15);
}

int main(void)
{        
 	u32 total,free;
  	u8 updateflag=0;	
	u32 t;
	u8 res=0;	
	u8 flashtemp[SIZE]={0};
	char updatepath[64];
	u8 databuff[4];
	DIR dir;
	FILINFO fileinfo;
	FIL *filp=NULL;
	u8 *fn;
	u8 *strchr_pointer;
	long oldversion,newversion;
	int len=0;
	u16 i=0,j=0;
	u32 temp;
	u32 fwaddr = FLASH_APP1_ADDR;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	montor_init();		//��ʼ�����ʹ�����ţ�����Ϊ�ߵ�ƽ
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	printf("start bootloader........\r\n");
	filp = (FIL*)mymalloc(SRAMIN,sizeof(FIL));
	if(filp==NULL)
	{
		printf("mymalloc filp error\r\n");
		goto exit;
	}
	printf("mymalloc filp successed!!!!!!\r\n");
	i=0;
	res = SD_Initialize();
	while(i<5 && res!=0)
	{
		res = SD_Initialize();
		delay_ms(300);
		i++;
	}
 	if(res)//��ⲻ��SD��
	{
		printf("SD Card Error!\r\n");
		delay_ms(300);					
		printf("Please Check!\r\n");
		delay_ms(300);
		goto exit;
	}
	printf("sd card init successed!!!!!!\r\n");
 	exfuns_init();							//Ϊfatfs��ر��������ڴ�				 
  	f_mount(fs[0],"0:",1); 					//����SD�� 

	if(exf_getfree("0",&total,&free))	//�õ�SD������������ʣ������
	{
		printf("SD Card Fatfs Error!\r\n");
		delay_ms(200);
		delay_ms(200);
		goto exit;
	}													  			    
	printf("FATFS OK!\r\n");	 
	printf("SD Total Size: %d MB",total>>10);	 
	printf("SD  Free Size: %d MB",free>>10); 
	res = f_open(filp,"0:SM3DP.bin",FA_READ);	//��ѯ�Ƿ���������
	if(res)
	{
		printf("res=%d line=%d\r\n",res,__LINE__);
		goto exit;
	}
	printf("line:%d\r\n",__LINE__);
	res = f_stat("0:SM3DP.bin",&fileinfo);
	if(res)
	{
		printf("res=%d line=%d\r\n",res,__LINE__);
		goto exit;
	}
	printf("fileinfo.fsize=%d line:%d\r\n",fileinfo.fsize,__LINE__);

	j=0;
	for(t=0;t<fileinfo.fsize;t+=4)
	{	
		f_lseek(filp,t);
		res = f_read(filp,databuff,4,&br);
		temp=(u32)databuff[3]<<24;   
		temp|=(u32)databuff[2]<<16;    
		temp|=(u32)databuff[1]<<8;
		temp|=(u32)databuff[0];	  
		//dfu+=4;//ƫ��4���ֽ�
		iapbuf[i++]=temp;	
		j++;
		if(i==512)
		{
			printf("j=%d line:%d\r\n",j,__LINE__);
			i=0; 
			STMFLASH_Write(fwaddr,iapbuf,512);
			fwaddr+=2048;//ƫ��2048   512*4=2048
		}
	}
	printf("line:%d\r\n",__LINE__);
	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ.  
	//�����ļ�������ֹ�´ο����ٴ�����

	f_rename("0:SM3DP.bin","0:OLDSM3DP.bin");

	printf("��ʼִ����FLASH�û����룡������\r\n");
	if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)		//�ж��Ƿ�Ϊ0x08xxxxxx.
	{
		printf("line:%d\r\n",__LINE__);
		iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
	}
	else 
	{
		printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
	}
	while(1)
	{
		;
	}
	exit:
	myfree(SRAMIN,filp);
	printf("exit\r\n");
	if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)		//�ж��Ƿ�Ϊ0x08xxxxxx.
	{
		printf("ִ��ԭ�û����룡������\r\n");
		iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
	}
}





