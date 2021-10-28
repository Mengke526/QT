#pragma once
#include<vector>
using namespace std;

class DSPControl
{
private:
public:
	vector<unsigned int> controlBits;
	DSPControl() {};
	void DSPControlInit(); // ��ʼ��ͨ������֡ͷ
	void DSPControlUpdata(double Tx, double Ty, double Ax, double Ay, double Wx, double Wy); // ����ͨ�����ݵ���������
	void DSPControlClear(); // ͨ�����ݵ�������������
};