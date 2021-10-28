#pragma once
#include<vector>
using namespace std;

class DSPControl
{
private:
public:
	vector<unsigned int> controlBits;
	DSPControl() {};
	void DSPControlInit(); // 初始化通信数据帧头
	void DSPControlUpdata(double Tx, double Ty, double Ax, double Ay, double Wx, double Wy); // 更新通信数据的力矩数据
	void DSPControlClear(); // 通信数据的力矩数据清零
};