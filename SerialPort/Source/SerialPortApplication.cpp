#include "SerialPortApplication.h"
#include <QMessageBox>
#include"ui_SerialPortApplication.h"

#pragma execution_character_set("UTF-8")

SerialPortApplication::SerialPortApplication(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //�����źźͲ�
    QObject::connect(&serial, &QSerialPort::readyRead, this, &SerialPortApplication::serialPort_readyRead);
    //���Ͱ���ʧ��
    ui.SendButton->setEnabled(false);
    //������Ĭ��ѡ��������һ�9600
    ui.BaudrateBox->setCurrentIndex(0);
}
void SerialPortApplication::serialPort_readyRead()
{
    //�ӽ��ջ������ж�ȡ����
    QByteArray buffer = serial.readAll();
    if (ui.HexcheckBox->isChecked())
    {
        //�ӽ����ж�ȡ��ǰ�յ�������
        QString recv = ui.ReceiveTextEdit->toPlainText();
        recv += buffer.toHex();
        //�����ǰ����ʾ
        ui.ReceiveTextEdit->clear();
        //������ʾ
        ui.ReceiveTextEdit->append(recv);
    }
    else
    {
        //�ӽ����ж�ȡ��ǰ�յ�������
        QString recv = ui.ReceiveTextEdit->toPlainText();
        recv += QString(buffer);
        //�����ǰ����ʾ
        ui.ReceiveTextEdit->clear();
        //������ʾ
        ui.ReceiveTextEdit->append(recv);
    }
}

void SerialPortApplication::on_SearchButton_clicked()
{
    ui.PortNameBox->clear();
    //ͨ��QSerialPortInfo���ҿ��ô���
    foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
    {
        ui.PortNameBox->addItem(info.portName());
    }
}

void SerialPortApplication::on_OpenButton_clicked()
{
    if (ui.OpenButton->text() == QString("�򿪴���"))
    {
        //���ô�����
        serial.setPortName(ui.PortNameBox->currentText());
        //���ò�����
        serial.setBaudRate(ui.BaudrateBox->currentText().toInt());
        //��������λ��
        switch (ui.DataBitsBox->currentText().toInt())
        {
        case 8:
            serial.setDataBits(QSerialPort::Data8);
            break;
        default:
            break;
        }
        //������żУ��
        switch (ui.ParityBox->currentText().toInt())
        {
        case 0:
            serial.setParity(QSerialPort::NoParity);
            break;
        default:
            break;
        }
        //����ֹͣλ
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
        //����������
        serial.setFlowControl(QSerialPort::NoFlowControl);
        //�򿪴���
        if (!serial.open(QIODevice::ReadWrite))
        {
            QMessageBox::about(NULL, "��ʾ", "�޷��򿪴��ڣ�");
            return;
        }
        //�����˵��ؼ�ʧ�� 
        ui.PortNameBox->setEnabled(false);
        ui.BaudrateBox->setEnabled(false);
        ui.DataBitsBox->setEnabled(false);
        ui.ParityBox->setEnabled(false);
        ui.StopBitsBox->setEnabled(false);
        //���İ����ı�
        ui.OpenButton->setText(QString("�رմ���"));
        //���Ͱ���ʹ��
        ui.SendButton->setEnabled(true);
    }
    else
    {
        //�رմ���
        serial.close();
        //�����˵��ؼ�ʹ��
        ui.PortNameBox->setEnabled(true);
        ui.BaudrateBox->setEnabled(true);
        ui.DataBitsBox->setEnabled(true);
        ui.ParityBox->setEnabled(true);
        ui.StopBitsBox->setEnabled(true);
        //���İ����ı�
        ui.OpenButton->setText(QString("�򿪴���"));
        //���Ͱ���ʧ��
        ui.SendButton->setEnabled(false);
    }
}

void SerialPortApplication::on_SendButton_clicked()
{
    //��ȡ�����ϵ����ݲ�ת����utf8
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