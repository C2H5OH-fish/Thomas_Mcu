/*====================================================================
* Author   : Nanking
* Date     : 2022-7-06
* Function : Plaese read "readme.txt"

All rights reserved; 
those responsible for unauthorized reproduction will be prosecuted.
=====================================================================*/

#include <REGX51.H>


//ʹ������L9110Sģ����������
//-------------------------
sbit A0_1A = P1^0; //��ǰ��
sbit A0_1B = P1^1;
//-------------------------
sbit B0_1A = P1^2; //��ǰ��
sbit B0_1B = P1^3;
//-------------------------
sbit A1_1A = P1^4; //�����
sbit A1_1B = P1^5;
//-------------------------
sbit B1_1A = P1^6; //�Һ���
sbit B1_1B = P1^7;
/*====================================
A_1A����ߵ�ƽ��A_1B����͵�ƽ�����A��ת
A_1A����͵�ƽ��A_1B����ߵ�ƽ�����A��ת
B_1A����ߵ�ƽ��B_1B����͵�ƽ�����B��ת
B_1A����͵�ƽ��B_1B����ߵ�ƽ�����B��ת
=====================================*/


#define uint  unsigned int    //�ض����޷�����������
#define uchar unsigned char   //�ض����޷����ַ�����
	

//�����ٶȴ�С��ֵ
uchar gao_num=0,di_num=4;
uchar i; //����ٺ����й�

#define ShowPort P0                 //�����������ʾ�˿�
uchar code  LedShowData[]={0x03,0x9F,0x25,0x0D,0x99,   
													 0x49,0x41,0x1F,0x01,0x19};  
//�����������ʾ����,0,1,2,3,4,5,6,7,8,9

													 
typedef unsigned char u8;
u8 receivedate;//���յ�������
/*���ڳ�ʼ������*/
void UartConfiguration()  
{
    TMOD = 0x20;     //���ü�����1�Ĺ�����ʽ2
    TH1  = 0xfd;	   //���ü�����1�ĳ�ֵ������������,//11.0592M����9600������
    TL1  = 0xfd;		 //���ü�����1�ĳ�ֵ������������
    PCON = 0x00;     //�����ʱ���0x00���ӱ�	 0x80�ӱ�
    SCON = 0x50;		 //���ù�����ʽ1 ������������
    EA   = 1;		     //�������ж�
    ES   = 1;			   //�������ڽ����ж�
    TR1  = 1;			   //������1��ʼ����
}


//С��ϵ�й���
//-----------------------
void FrontGo()     //ǰ��
{
	A0_1A = 0;  //��ǰ��ǰ��
	A0_1B = 1;
			
	B0_1A = 0;  //��ǰ��ǰ��
	B0_1B = 1;
			  
	A1_1A = 0;  //�����ǰ��
	A1_1B = 1;
			
	B1_1A = 0;  //�Һ���ǰ��
	B1_1B = 1;
}
//-----------------------
void BackGo()      //����
{
	A0_1A = 1;  //��ǰ�ֺ���
	A0_1B = 0;
			
	B0_1A = 1;  //��ǰ�ֺ���
	B0_1B = 0;
			  
	A1_1A = 1;  //����ֺ���
	A1_1B = 0;
			
	B1_1A = 1;  //�Һ��ֺ���
	B1_1B = 0;
}
//------------------------
void LeftGo()       //��ת
{
	A0_1A = 1;  //��ǰ�ֺ���
	A0_1B = 0;
			
	B0_1A = 0;  //��ǰ��ǰ��
	B0_1B = 1;
			  
	A1_1A = 1;  //����ֺ���
	A1_1B = 0;
			
	B1_1A = 0;  //�Һ���ǰ��
	B1_1B = 1;
}
//------------------------
void RightGo()      //��ת
{
	A0_1A = 0;  //��ǰ��ǰ��
	A0_1B = 1;
			
	B0_1A = 1;  //��ǰ�ֺ���
	B0_1B = 0;
			  
	A1_1A = 0;  //�����ǰ��
	A1_1B = 1;
			
	B1_1A = 1;  //�Һ��ֺ���
	B1_1B = 0;
}
//------------------------
void StopGo()       //ɲ��
{
	A0_1A = 0;  //��ǰ��ɲ��
	A0_1B = 0;
			
	B0_1A = 0;  //��ǰ��ɲ��
	B0_1B = 0;
			  
	A1_1A = 0;  //�����ɲ��
	A1_1B = 0;
			
	B1_1A = 0;  //�Һ���ɲ��
	B1_1B = 0;
}
//------------------------
void FrontChangeSpeed()
{
	if(di_num !=0)
	{
		for(i=0;i<di_num;i++)
		{
			A0_1B = 0;//��ǰ��								//ʵ��PWM�źŵ͵�ƽ���
			B0_1B = 0;//��ǰ��	
			A1_1B = 0;//�����
			B1_1B = 0;//�Һ���
			
		}
	}
	for(i=0;i<gao_num;i++)
	{
		A0_1B = 1;								 //ʵ��PWM�źŸߵ�ƽ���
		B0_1B = 1;
		A1_1B = 1;
		B1_1B = 1;
		
	}
}
//-----------------------
void BackChangeSpeed()
{
	if(di_num !=0)
	{
		for(i=0;i<di_num;i++)
		{
			A0_1A = 0;//��ǰ��								//ʵ��PWM�źŵ͵�ƽ���
			B0_1A = 0;//��ǰ��	
			A1_1A = 0;//�����
			B1_1A = 0;//�Һ���
			
		}
	}
	for(i=0;i<gao_num;i++)
	{
		A0_1A = 1;								 //ʵ��PWM�źŸߵ�ƽ���
		B0_1A = 1;
		A1_1A = 1;
		B1_1A = 1;
		
	}
}

/*==================================������================================*/
void main()
{
  UartConfiguration();
  //sendDate('1');  //�����ַ�1
	ShowPort=LedShowData[0];
	
 	while(1)
	{
		switch(receivedate)
		{	
			//----------------��������------------------
			case 0x11:    //ǰ��
				FrontGo();
				ShowPort=LedShowData[1];  //�������ʾ����1
			  break;	 
			
			case 0x12:    //����
				BackGo();
				ShowPort=LedShowData[1];  //�������ʾ����1
			  break;	 
				
			case 0x13:    //��ת
				LeftGo();
				ShowPort=LedShowData[1];  //�������ʾ����1
			  break;	 
			
			case 0x14:    //��ת
				RightGo();
				ShowPort=LedShowData[1];  //�������ʾ����1
			  break;	 

			case 0x10:      //ɲ��
				StopGo();
				ShowPort=LedShowData[0];  //�����Ĭ������0
			  break;	
			//---------------ǰ������ռ�ձȵ���---------------------------
			case 0x20: 
				gao_num=0;		 //PWM �ź��иߵ�ƽ����ʱ���־Ϊ0
				di_num=4;      //PWM �ź��е͵�ƽ����ʱ���־Ϊ4����ʱ�ٶ�Ϊ0
				FrontChangeSpeed();//ǰ������
				ShowPort=LedShowData[2];  //�������ʾ����2
				break;
			
			case 0x21:    
				gao_num=1;		 //PWM �ź��иߵ�ƽ����ʱ���־Ϊ1
				di_num=3;      //PWM �ź��е͵�ƽ����ʱ���־Ϊ3����ʱ�ٶ�����
				FrontChangeSpeed();//ǰ������
				ShowPort=LedShowData[2];  //�������ʾ����2
			  break;	 
			
			case 0x22:    
				gao_num=2;		 //PWM �ź��иߵ�ƽ����ʱ���־Ϊ2
				di_num=2;      //PWM �ź��е͵�ƽ����ʱ���־Ϊ2����ʱ�ٶȽ���
				FrontChangeSpeed();//ǰ������
				ShowPort=LedShowData[2];  //�������ʾ����2
				break;
			
			case 0x23:    
				gao_num=3;		 //PWM �ź��иߵ�ƽ����ʱ���־Ϊ3
				di_num=1;      //PWM �ź��е͵�ƽ����ʱ���־Ϊ1����ʱ�ٶȽϿ�
				FrontChangeSpeed();//ǰ������
				ShowPort=LedShowData[2];  //�������ʾ����2
				break;
			
			case 0x24:    
				gao_num=4;		 //PWM �ź��иߵ�ƽ����ʱ���־Ϊ4
				di_num=0;      //PWM �ź��е͵�ƽ����ʱ���־Ϊ0����ʱ�ٶ����
				FrontChangeSpeed();//ǰ������
				ShowPort=LedShowData[2];  //�������ʾ����2
				break;
			
			case 0x25:
				gao_num=0;		 //PWM �ź��иߵ�ƽ����ʱ���־Ϊ0
				di_num=4;      //PWM �ź��е͵�ƽ����ʱ���־Ϊ4����ʱ�ٶ�Ϊ0
				BackChangeSpeed();//���˵���
				ShowPort=LedShowData[2];  //�������ʾ����2
				break;
			
			case 0x26:    
				gao_num=1;		 //PWM �ź��иߵ�ƽ����ʱ���־Ϊ1
				di_num=3;      //PWM �ź��е͵�ƽ����ʱ���־Ϊ3����ʱ�ٶ�����
				BackChangeSpeed();//���˵���
				ShowPort=LedShowData[2];  //�������ʾ����2
			  break;	 
			
			case 0x27:    
				gao_num=2;		 //PWM �ź��иߵ�ƽ����ʱ���־Ϊ2
				di_num=2;      //PWM �ź��е͵�ƽ����ʱ���־Ϊ2����ʱ�ٶȽ���
				BackChangeSpeed();//���˵���
				ShowPort=LedShowData[2];  //�������ʾ����2
				break;
			
			case 0x28:     
				gao_num=3;		 //PWM �ź��иߵ�ƽ����ʱ���־Ϊ3
				di_num=1;      //PWM �ź��е͵�ƽ����ʱ���־Ϊ1����ʱ�ٶȽϿ�
				BackChangeSpeed();//���˵���
				ShowPort=LedShowData[2];  //�������ʾ����2
				break;
			
			case 0x29:    
				gao_num=4;		 //PWM �ź��иߵ�ƽ����ʱ���־Ϊ4
				di_num=0;      //PWM �ź��е͵�ƽ����ʱ���־Ϊ0����ʱ�ٶ����
				BackChangeSpeed();//���˵���
				ShowPort=LedShowData[2];  //�������ʾ����2
				break;
		 }
   }
}
 
 
/*===============================�жϺ���================================================*/
void Uart() interrupt 4
{
    //u8 date;
    receivedate=SBUF;        //ȡ�����յ�������
    RI=0;			  //��������жϱ�־λ
 
    //�յ���������receivedate
	SBUF = receivedate;    //�ֻ�������ģ��ͨѶʱ�����ǿ��Դ򿪵��Դ��ڵ��Թ��߲鿴���ǵķ���ֵ
    while(!TI);
    TI = 0; 
}


	

	    	
	    	
