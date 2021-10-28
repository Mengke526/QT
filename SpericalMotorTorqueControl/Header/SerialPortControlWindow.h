#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_SerialPortControlWindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "DSPControl.h"

class SerialPortControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    SerialPortControlWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::SerialPortControlWindowClass ui; // ui对象
    QSerialPort serial; // 串口通信对象
    DSPControl dspctrl; // 通信数据对象
    void SerialPortSend(); // 串口输出力矩数据

private slots:
    void serialPort_readyRead(); // 接收串口数据
    void on_SearchButton_clicked(); // 串口搜索按键按下，搜索所有的端口
    void on_OpenButton_clicked(); // 串口打开按键按下，配置串口参数打开串口
    void on_SendButton_clicked(); // 发送按键按下，串口输出力矩数据
    void on_ZeroButton_clicked(); // 清零按键按下，力矩清零并输出数据
    void TxslotDoubleSpinBox_Slider(); // TxDoubleSpinBox变动时，TxSlider改变
    void TxslotSlider_DoubleSpinBox(); // TxSlider变动时，TxDoubleSpinBox改变
    void TyslotDoubleSpinBox_Slider(); // TyDoubleSpinBox变动时，TySlider改变
    void TyslotSlider_DoubleSpinBox(); // TySlider变动时，TyDoubleSpinBox改变
};