#include "DSPControl.h"

// ��ʼ��ͨ������֡ͷ
void DSPControl::DSPControlInit()
{
	/*
	* 0,1,2: ֡ͷBD,DB,0A
	* 3,4: 10000*(Tx+0.5)��8λ����8λ
	* 5,6: 10000*(Ty+0.5)��8λ����8λ
	* 7,8: 10000*Ax��8λ����8λ
	* 9,10: 10000*Ay��8λ����8λ
	* 11,12: 10000*Wx��8λ����8λ
	* 13,14: 10000*Wy��8λ����8λ
	* 15:0x0001->PID����������,0x0002->��������,0x0004->��Ծ�ź�,0x0008->�����ź�
	*/
	controlBits.resize(16);
	controlBits[0] = 0x00BD;
	controlBits[1] = 0x00DB;
	controlBits[2] = 0x000A;
}

// ����ͨ�����ݵ���������
void DSPControl::DSPControlUpdata(double Tx, double Ty, double Ax, double Ay, double Wx, double Wy)
{
	int Tx_hex = 10000 * Tx;
	int Ty_hex = 10000 * Ty;
	int Ax_hex = 10000 * Ax;
	int Ay_hex = 10000 * Ay;
	int Wx_hex = 10000 * Wx;
	int Wy_hex = 10000 * Wy;
	controlBits[3] = Tx_hex & 0x00FF;
	controlBits[4] = (Tx_hex >> 8) & 0x00FF;
	controlBits[5] = Ty_hex & 0x00FF;
	controlBits[6] = (Ty_hex >> 8) & 0x00FF;
	controlBits[7] = Ax_hex & 0x00FF;
	controlBits[8] = (Ax_hex >> 8) & 0x00FF;
	controlBits[9] = Ay_hex & 0x00FF;
	controlBits[10] = (Ay_hex >> 8) & 0x00FF;
	controlBits[11] = Wx_hex & 0x00FF;
	controlBits[12] = (Wx_hex >> 8) & 0x00FF;
	controlBits[13] = Wy_hex & 0x00FF;
	controlBits[14] = (Wy_hex >> 8) & 0x00FF;
}

// ͨ�����ݵ�������������
void DSPControl::DSPControlClear()
{
	controlBits[3] = 0x0000;
	controlBits[4] = 0x0000;
	controlBits[5] = 0x0000;
	controlBits[6] = 0x0000;
	controlBits[7] = 0x0000;
	controlBits[8] = 0x0000;
	controlBits[9] = 0x0000;
	controlBits[10] = 0x0000;
	controlBits[11] = 0x0000;
	controlBits[12] = 0x0000;
	controlBits[13] = 0x0000;
	controlBits[14] = 0x0000;
}