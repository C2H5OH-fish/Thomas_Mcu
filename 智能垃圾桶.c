/*====================================================================
* Author   : Nanking
* Date     : 2022-6-23
* Function : Plaese read "readme.txt"

All rights reserved; 
those responsible for unauthorized reproduction will be prosecuted.
=====================================================================*/

#include<reg52.h>

sbit led1    = P1^0;  //指示灯
sbit Trig    = P1^5;  //SR04_Trig
sbit Echo    = P1^6;  //SR04_Echo
sbit sg90    = P1^1;  //sg90信号端
sbit key     = P1^2;  //按键
sbit vibrate = P3^2;  //震动传感器
sbit beep    = P2^0;  //蜂鸣器信号端

char jiaodu;          //角度标志
char jiaodu_last;     //角度上次的标志
char cnt         = 0; //计时标志
char SignVibrate = 0; //震动传感器标志

void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	i = 2;
	while (--i);
}

void Delay2000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	//_nop_();
	i = 15;
	j = 2;
	k = 235;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay150ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	//_nop_();
	i = 2;
	j = 13;
	k = 237;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void Time0Init()
{
	//1.配置定时器0工作模式16位计时
	TMOD &= 0xF0;
	TMOD |= 0x01;
	//2.给初值，定一个0.5ms出来
	TL0 = 0x33;
	TH0 = 0xFE;
	//3.开始计时
	TR0 = 1;
	TF0 = 0;
	//4.打开定时器0中断
	ET0 = 1;
	//5.打开总中断EA
	EA = 1;
}

void Time1Init()
{
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;
	TH1  = 0;
  TL1  = 0;	
	//设置定时器1,初始值设定为0，开始数数，不着急启动定时器0
}

void StartSR04()
{
	Trig = 0;
	Trig = 1;
	Delay10us();
	Trig = 0;
}

double get_distance()
{
	double time;  //时间
	//定时器数据清0，以便下一次测距
	TH1  = 0;
	TL1  = 0;	
	//1.Trig,给Trig端口至少10us的高电平
	StartSR04();
	//2.Echo由低电平跳转到高电平，表示开始发送波，波发出的那下，启动定时器
	while(Echo == 0);
	TR1 = 1;
	//3.由高电平跳转回低电平，表示波回来了，波回来的那下，关闭定时器
	while(Echo == 1);
	TR1 = 0;
	//4.计算中间经历多少时间
	time = (TH1 * 256 + TL1)*1.085;  //单位为微秒us
	//5.距离 = 速度（340m/s）* 时间 / 2
	return (0.034 * time /2);
}

void initSG90_0()
{
	jiaodu = 1;  //初始角度为0，0.5ms,溢出1就是0.5，高电平
	cnt = 0;
	sg90 = 1;   //一开始从高电平开始
}

void kaigai()
{
	char n;
	//开盖状态，灯亮，舵机90度
			led1 = 0;
			jiaodu = 3; //90度，1.5ms的高电平
			if(jiaodu_last != jiaodu)
			{
				cnt = 0;
				beep = 0;
				for(n=0;n<2;n++)
				Delay150ms();	
				beep =1;
				Delay2000ms();
			}
			jiaodu_last = jiaodu;
}

void guangai()
{
	//关盖状态，灯灭，舵机0度
			led1 = 1;
			jiaodu = 1; //0度，0.5ms的高电平
			jiaodu_last = jiaodu;
			cnt    = 0;
			Delay150ms();
}

void EX0_Init()
{
	EX0 = 1; //外部中断
	IT0 = 0; //低电平触发
}
void main()
{
	
	double dis;   //距离
	
	Time0Init();
	Time1Init();
	EX0_Init();
	
	initSG90_0(); //舵机初始化
	
	while(1)
	{
		//超声波测距
		dis = get_distance();
		if(dis <= 10 || key == 0 || SignVibrate == 1) //开盖条件
		{
			kaigai();  //开盖
			SignVibrate = 0;  //恢复默认状态
		}
		else
		{
			guangai(); //关盖
		}
		
	}
}

void T0_time() interrupt 1
{
	cnt++; //统计爆表的次数
	//重新给初值
	TL0 = 0x33;
	TH0 = 0xFE;
	
	//控制PWM波
	if(cnt < jiaodu)	//爆表了n次，经历nms,取角度所用
	{
		sg90 = 1;	
	}
	else
	{
		sg90 = 0;
	}
	
	//计数20ms
	if(cnt == 40) //爆表了40次，经历了20ms
	{
		cnt = 0;
		sg90 = 1;
	}
}

void EX0_Handler() interrupt 0
{
	SignVibrate = 1;
}