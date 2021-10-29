#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_SerialPortControlWindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtConcurrent/qtconcurrentrun.h>
#include <qthread.h>
#include <time.h>
#include <math.h>
#include "DSPControl.h"

class SerialPortControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    SerialPortControlWindow(QWidget *parent = Q_NULLPTR);

private:
    bool runflag; // ѭ���������ݱ�־λ
    Ui::SerialPortControlWindowClass ui; // ui����
    QSerialPort serial; // ����ͨ�Ŷ���
    DSPControl dspctrl; // ͨ�����ݶ���
    void SerialPortSend(); // ���������������
    void run(); // ���߳���������
    void loop();
    clock_t start;

private slots:
    void serialPort_readyRead(); // ���մ�������
    void on_SearchButton_clicked(); // ���������������£��������еĶ˿�
    void on_OpenButton_clicked(); // ���ڴ򿪰������£����ô��ڲ����򿪴���
    void on_SendButton_clicked(); // ���Ͱ������£����������������
    void on_ZeroButton_clicked(); // ���㰴�����£��������㲢�������
    void TxslotDoubleSpinBox_Slider(); // TxDoubleSpinBox�䶯ʱ��TxSlider�ı�
    void TxslotSlider_DoubleSpinBox(); // TxSlider�䶯ʱ��TxDoubleSpinBox�ı�
    void TyslotDoubleSpinBox_Slider(); // TyDoubleSpinBox�䶯ʱ��TySlider�ı�
    void TyslotSlider_DoubleSpinBox(); // TySlider�䶯ʱ��TyDoubleSpinBox�ı�
};