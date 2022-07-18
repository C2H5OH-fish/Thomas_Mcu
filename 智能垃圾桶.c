/*====================================================================
* Author   : Nanking
* Date     : 2022-6-23
* Function : Plaese read "readme.txt"

All rights reserved; 
those responsible for unauthorized reproduction will be prosecuted.
=====================================================================*/

#include<reg52.h>

sbit led1    = P1^0;  //ָʾ��
sbit Trig    = P1^5;  //SR04_Trig
sbit Echo    = P1^6;  //SR04_Echo
sbit sg90    = P1^1;  //sg90�źŶ�
sbit key     = P1^2;  //����
sbit vibrate = P3^2;  //�𶯴�����
sbit beep    = P2^0;  //�������źŶ�

char jiaodu;          //�Ƕȱ�־
char jiaodu_last;     //�Ƕ��ϴεı�־
char cnt         = 0; //��ʱ��־
char SignVibrate = 0; //�𶯴�������־

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
	//1.���ö�ʱ��0����ģʽ16λ��ʱ
	TMOD &= 0xF0;
	TMOD |= 0x01;
	//2.����ֵ����һ��0.5ms����
	TL0 = 0x33;
	TH0 = 0xFE;
	//3.��ʼ��ʱ
	TR0 = 1;
	TF0 = 0;
	//4.�򿪶�ʱ��0�ж�
	ET0 = 1;
	//5.�����ж�EA
	EA = 1;
}

void Time1Init()
{
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x10;
	TH1  = 0;
  TL1  = 0;	
	//���ö�ʱ��1,��ʼֵ�趨Ϊ0����ʼ���������ż�������ʱ��0
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
	double time;  //ʱ��
	//��ʱ��������0���Ա���һ�β��
	TH1  = 0;
	TL1  = 0;	
	//1.Trig,��Trig�˿�����10us�ĸߵ�ƽ
	StartSR04();
	//2.Echo�ɵ͵�ƽ��ת���ߵ�ƽ����ʾ��ʼ���Ͳ��������������£�������ʱ��
	while(Echo == 0);
	TR1 = 1;
	//3.�ɸߵ�ƽ��ת�ص͵�ƽ����ʾ�������ˣ������������£��رն�ʱ��
	while(Echo == 1);
	TR1 = 0;
	//4.�����м侭������ʱ��
	time = (TH1 * 256 + TL1)*1.085;  //��λΪ΢��us
	//5.���� = �ٶȣ�340m/s��* ʱ�� / 2
	return (0.034 * time /2);
}

void initSG90_0()
{
	jiaodu = 1;  //��ʼ�Ƕ�Ϊ0��0.5ms,���1����0.5���ߵ�ƽ
	cnt = 0;
	sg90 = 1;   //һ��ʼ�Ӹߵ�ƽ��ʼ
}

void kaigai()
{
	char n;
	//����״̬�����������90��
			led1 = 0;
			jiaodu = 3; //90�ȣ�1.5ms�ĸߵ�ƽ
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
	//�ظ�״̬�����𣬶��0��
			led1 = 1;
			jiaodu = 1; //0�ȣ�0.5ms�ĸߵ�ƽ
			jiaodu_last = jiaodu;
			cnt    = 0;
			Delay150ms();
}

void EX0_Init()
{
	EX0 = 1; //�ⲿ�ж�
	IT0 = 0; //�͵�ƽ����
}
void main()
{
	
	double dis;   //����
	
	Time0Init();
	Time1Init();
	EX0_Init();
	
	initSG90_0(); //�����ʼ��
	
	while(1)
	{
		//���������
		dis = get_distance();
		if(dis <= 10 || key == 0 || SignVibrate == 1) //��������
		{
			kaigai();  //����
			SignVibrate = 0;  //�ָ�Ĭ��״̬
		}
		else
		{
			guangai(); //�ظ�
		}
		
	}
}

void T0_time() interrupt 1
{
	cnt++; //ͳ�Ʊ���Ĵ���
	//���¸���ֵ
	TL0 = 0x33;
	TH0 = 0xFE;
	
	//����PWM��
	if(cnt < jiaodu)	//������n�Σ�����nms,ȡ�Ƕ�����
	{
		sg90 = 1;	
	}
	else
	{
		sg90 = 0;
	}
	
	//����20ms
	if(cnt == 40) //������40�Σ�������20ms
	{
		cnt = 0;
		sg90 = 1;
	}
}

void EX0_Handler() interrupt 0
{
	SignVibrate = 1;
}