#include "SerialPortApplication.h"
#include <QMessageBox>
#include"ui_SerialPortApplication.h"

#pragma execution_character_set("UTF-8")

SerialPortApplication::SerialPortApplication(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //连接信号和槽
    QObject::connect(&serial, &QSerialPort::readyRead, this, &SerialPortApplication::serialPort_readyRead);
    //发送按键失能
    ui.SendButton->setEnabled(false);
    //波特率默认选择下拉第一项：9600
    ui.BaudrateBox->setCurrentIndex(0);
}
void SerialPortApplication::serialPort_readyRead()
{
    //从接收缓冲区中读取数据
    QByteArray buffer = serial.readAll();
    if (ui.HexcheckBox->isChecked())
    {
        //从界面中读取以前收到的数据
        QString recv = ui.ReceiveTextEdit->toPlainText();
        recv += buffer.toHex();
        //清空以前的显示
        ui.ReceiveTextEdit->clear();
        //重新显示
        ui.ReceiveTextEdit->append(recv);
    }
    else
    {
        //从界面中读取以前收到的数据
        QString recv = ui.ReceiveTextEdit->toPlainText();
        recv += QString(buffer);
        //清空以前的显示
        ui.ReceiveTextEdit->clear();
        //重新显示
        ui.ReceiveTextEdit->append(recv);
    }
}

void SerialPortApplication::on_SearchButton_clicked()
{
    ui.PortNameBox->clear();
    //通过QSerialPortInfo查找可用串口
    foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
    {
        ui.PortNameBox->addItem(info.portName());
    }
}

void SerialPortApplication::on_OpenButton_clicked()
{
    if (ui.OpenButton->text() == QString("打开串口"))
    {
        //设置串口名
        serial.setPortName(ui.PortNameBox->currentText());
        //设置波特率
        serial.setBaudRate(ui.BaudrateBox->currentText().toInt());
        //设置数据位数
        switch (ui.DataBitsBox->currentText().toInt())
        {
        case 8:
            serial.setDataBits(QSerialPort::Data8);
            break;
        default:
            break;
        }
        //设置奇偶校验
        switch (ui.ParityBox->currentText().toInt())
        {
        case 0:
            serial.setParity(QSerialPort::NoParity);
            break;
        default:
            break;
        }
        //设置停止位
        switch (ui.StopBitsBox->currentText().toInt())
        {
        case 1:
            serial.setStopBits(QSerialPort::OneStop);
            break;
        case 2:
            serial.setStopBits(QSerialPort::TwoStop);
            break;
        default:
            break;
        }
        //设置流控制
        serial.setFlowControl(QSerialPort::NoFlowControl);
        //打开串口
        if (!serial.open(QIODevice::ReadWrite))
        {
            QMessageBox::about(NULL, "提示", "无法打开串口！");
            return;
        }
        //下拉菜单控件失能 
        ui.PortNameBox->setEnabled(false);
        ui.BaudrateBox->setEnabled(false);
        ui.DataBitsBox->setEnabled(false);
        ui.ParityBox->setEnabled(false);
        ui.StopBitsBox->setEnabled(false);
        //更改按键文本
        ui.OpenButton->setText(QString("关闭串口"));
        //发送按键使能
        ui.SendButton->setEnabled(true);
    }
    else
    {
        //关闭串口
        serial.close();
        //下拉菜单控件使能
        ui.PortNameBox->setEnabled(true);
        ui.BaudrateBox->setEnabled(true);
        ui.DataBitsBox->setEnabled(true);
        ui.ParityBox->setEnabled(true);
        ui.StopBitsBox->setEnabled(true);
        //更改按键文本
        ui.OpenButton->setText(QString("打开串口"));
        //发送按键失能
        ui.SendButton->setEnabled(false);
    }
}

void SerialPortApplication::on_SendButton_clicked()
{
    //获取界面上的数据并转换成utf8
    QByteArray temp = ui.SendTextEdit->toPlainText().toUtf8();
    QByteArray data;
    int x = 0;
    for (int i = 0; i < temp.size(); i++) {
        x *= 16;
        if (temp[i] >= 'a')
        {
            x += temp[i] - 'a' + 10;
        }
        else if (temp[i] >= 'A')
        {
            x += temp[i] - 'A' + 10;
        }
        else
        {
            x += temp[i] - '0' ;
        }
    }
    data.push_back(x);
    serial.write(data);
}

void SerialPortApplication::on_ClearButton_clicked()
{
    ui.ReceiveTextEdit->clear();
}