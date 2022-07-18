/*====================================================================
* Author   : Nanking
* Date     : 2022-7-06
* Function : Plaese read "readme.txt"

All rights reserved; 
those responsible for unauthorized reproduction will be prosecuted.
=====================================================================*/

#include <REGX51.H>


//使用两个L9110S模块驱动轮子
//-------------------------
sbit A0_1A = P1^0; //左前轮
sbit A0_1B = P1^1;
//-------------------------
sbit B0_1A = P1^2; //右前轮
sbit B0_1B = P1^3;
//-------------------------
sbit A1_1A = P1^4; //左后轮
sbit A1_1B = P1^5;
//-------------------------
sbit B1_1A = P1^6; //右后轮
sbit B1_1B = P1^7;
/*====================================
A_1A输入高电平，A_1B输入低电平，电机A正转
A_1A输入低电平，A_1B输入高电平，电机A反转
B_1A输入高电平，B_1B输入低电平，电机B正转
B_1A输入低电平，B_1B输入高电平，电机B反转
=====================================*/


#define uint  unsigned int    //重定义无符号整数类型
#define uchar unsigned char   //重定义无符号字符类型
	

//定义速度大小阈值
uchar gao_num=0,di_num=4;
uchar i; //与调速函数有关

#define ShowPort P0                 //定义数码管显示端口
uchar code  LedShowData[]={0x03,0x9F,0x25,0x0D,0x99,   
													 0x49,0x41,0x1F,0x01,0x19};  
//定义数码管显示数据,0,1,2,3,4,5,6,7,8,9

													 
typedef unsigned char u8;
u8 receivedate;//接收到的数据
/*串口初始化配置*/
void UartConfiguration()  
{
    TMOD = 0x20;     //设置计数器1的工作方式2
    TH1  = 0xfd;	   //设置计数器1的初值，决定波特率,//11.0592M晶振，9600波特率
    TL1  = 0xfd;		 //设置计数器1的初值，决定波特率
    PCON = 0x00;     //波特率倍增0x00不加倍	 0x80加倍
    SCON = 0x50;		 //设置工作方式1 开启接受允许
    EA   = 1;		     //开启总中断
    ES   = 1;			   //开启串口接受中断
    TR1  = 1;			   //计数器1开始运行
}


//小车系列功能
//-----------------------
void FrontGo()     //前进
{
	A0_1A = 0;  //左前轮前进
	A0_1B = 1;
			
	B0_1A = 0;  //右前轮前进
	B0_1B = 1;
			  
	A1_1A = 0;  //左后轮前进
	A1_1B = 1;
			
	B1_1A = 0;  //右后轮前进
	B1_1B = 1;
}
//-----------------------
void BackGo()      //后退
{
	A0_1A = 1;  //左前轮后退
	A0_1B = 0;
			
	B0_1A = 1;  //右前轮后退
	B0_1B = 0;
			  
	A1_1A = 1;  //左后轮后退
	A1_1B = 0;
			
	B1_1A = 1;  //右后轮后退
	B1_1B = 0;
}
//------------------------
void LeftGo()       //左转
{
	A0_1A = 1;  //左前轮后退
	A0_1B = 0;
			
	B0_1A = 0;  //右前轮前进
	B0_1B = 1;
			  
	A1_1A = 1;  //左后轮后退
	A1_1B = 0;
			
	B1_1A = 0;  //右后轮前进
	B1_1B = 1;
}
//------------------------
void RightGo()      //右转
{
	A0_1A = 0;  //左前轮前进
	A0_1B = 1;
			
	B0_1A = 1;  //右前轮后退
	B0_1B = 0;
			  
	A1_1A = 0;  //左后轮前进
	A1_1B = 1;
			
	B1_1A = 1;  //右后轮后退
	B1_1B = 0;
}
//------------------------
void StopGo()       //刹车
{
	A0_1A = 0;  //左前轮刹车
	A0_1B = 0;
			
	B0_1A = 0;  //右前轮刹车
	B0_1B = 0;
			  
	A1_1A = 0;  //左后轮刹车
	A1_1B = 0;
			
	B1_1A = 0;  //右后轮刹车
	B1_1B = 0;
}
//------------------------
void FrontChangeSpeed()
{
	if(di_num !=0)
	{
		for(i=0;i<di_num;i++)
		{
			A0_1B = 0;//左前轮								//实现PWM信号低电平输出
			B0_1B = 0;//右前轮	
			A1_1B = 0;//左后轮
			B1_1B = 0;//右后轮
			
		}
	}
	for(i=0;i<gao_num;i++)
	{
		A0_1B = 1;								 //实现PWM信号高电平输出
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
			A0_1A = 0;//左前轮								//实现PWM信号低电平输出
			B0_1A = 0;//右前轮	
			A1_1A = 0;//左后轮
			B1_1A = 0;//右后轮
			
		}
	}
	for(i=0;i<gao_num;i++)
	{
		A0_1A = 1;								 //实现PWM信号高电平输出
		B0_1A = 1;
		A1_1A = 1;
		B1_1A = 1;
		
	}
}

/*==================================主函数================================*/
void main()
{
  UartConfiguration();
  //sendDate('1');  //发送字符1
	ShowPort=LedShowData[0];
	
 	while(1)
	{
		switch(receivedate)
		{	
			//----------------基本功能------------------
			case 0x11:    //前进
				FrontGo();
				ShowPort=LedShowData[1];  //数码管显示命令1
			  break;	 
			
			case 0x12:    //后退
				BackGo();
				ShowPort=LedShowData[1];  //数码管显示命令1
			  break;	 
				
			case 0x13:    //左转
				LeftGo();
				ShowPort=LedShowData[1];  //数码管显示命令1
			  break;	 
			
			case 0x14:    //右转
				RightGo();
				ShowPort=LedShowData[1];  //数码管显示命令1
			  break;	 

			case 0x10:      //刹车
				StopGo();
				ShowPort=LedShowData[0];  //数码管默认命令0
			  break;	
			//---------------前进后退占空比调速---------------------------
			case 0x20: 
				gao_num=0;		 //PWM 信号中高电平持续时间标志为0
				di_num=4;      //PWM 信号中低电平持续时间标志为4，此时速度为0
				FrontChangeSpeed();//前进调速
				ShowPort=LedShowData[2];  //数码管显示命令2
				break;
			
			case 0x21:    
				gao_num=1;		 //PWM 信号中高电平持续时间标志为1
				di_num=3;      //PWM 信号中低电平持续时间标志为3，此时速度最慢
				FrontChangeSpeed();//前进调速
				ShowPort=LedShowData[2];  //数码管显示命令2
			  break;	 
			
			case 0x22:    
				gao_num=2;		 //PWM 信号中高电平持续时间标志为2
				di_num=2;      //PWM 信号中低电平持续时间标志为2，此时速度较慢
				FrontChangeSpeed();//前进调速
				ShowPort=LedShowData[2];  //数码管显示命令2
				break;
			
			case 0x23:    
				gao_num=3;		 //PWM 信号中高电平持续时间标志为3
				di_num=1;      //PWM 信号中低电平持续时间标志为1，此时速度较快
				FrontChangeSpeed();//前进调速
				ShowPort=LedShowData[2];  //数码管显示命令2
				break;
			
			case 0x24:    
				gao_num=4;		 //PWM 信号中高电平持续时间标志为4
				di_num=0;      //PWM 信号中低电平持续时间标志为0，此时速度最快
				FrontChangeSpeed();//前进调速
				ShowPort=LedShowData[2];  //数码管显示命令2
				break;
			
			case 0x25:
				gao_num=0;		 //PWM 信号中高电平持续时间标志为0
				di_num=4;      //PWM 信号中低电平持续时间标志为4，此时速度为0
				BackChangeSpeed();//后退调速
				ShowPort=LedShowData[2];  //数码管显示命令2
				break;
			
			case 0x26:    
				gao_num=1;		 //PWM 信号中高电平持续时间标志为1
				di_num=3;      //PWM 信号中低电平持续时间标志为3，此时速度最慢
				BackChangeSpeed();//后退调速
				ShowPort=LedShowData[2];  //数码管显示命令2
			  break;	 
			
			case 0x27:    
				gao_num=2;		 //PWM 信号中高电平持续时间标志为2
				di_num=2;      //PWM 信号中低电平持续时间标志为2，此时速度较慢
				BackChangeSpeed();//后退调速
				ShowPort=LedShowData[2];  //数码管显示命令2
				break;
			
			case 0x28:     
				gao_num=3;		 //PWM 信号中高电平持续时间标志为3
				di_num=1;      //PWM 信号中低电平持续时间标志为1，此时速度较快
				BackChangeSpeed();//后退调速
				ShowPort=LedShowData[2];  //数码管显示命令2
				break;
			
			case 0x29:    
				gao_num=4;		 //PWM 信号中高电平持续时间标志为4
				di_num=0;      //PWM 信号中低电平持续时间标志为0，此时速度最快
				BackChangeSpeed();//后退调速
				ShowPort=LedShowData[2];  //数码管显示命令2
				break;
		 }
   }
}
 
 
/*===============================中断函数================================================*/
void Uart() interrupt 4
{
    //u8 date;
    receivedate=SBUF;        //取出接收到的数据
    RI=0;			  //清除接受中断标志位
 
    //收到的数据是receivedate
	SBUF = receivedate;    //手机和蓝牙模块通讯时，我们可以打开电脑串口调试工具查看我们的发送值
    while(!TI);
    TI = 0; 
}


	

	    	
	    	
