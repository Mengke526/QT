#include "SerialPortControlWindow.h"
#include <QMessageBox>
#pragma execution_character_set("UTF-8")

// ��ʼ�������
SerialPortControlWindow::SerialPortControlWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //�����źźͲ�
    QObject::connect(&serial, &QSerialPort::readyRead, this, &SerialPortControlWindow::serialPort_readyRead);
    connect(ui.TxdoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(TxslotDoubleSpinBox_Slider()));
    connect(ui.TxSlider, SIGNAL(valueChanged(int)), this, SLOT(TxslotSlider_DoubleSpinBox()));
    connect(ui.TydoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(TyslotDoubleSpinBox_Slider()));
    connect(ui.TySlider, SIGNAL(valueChanged(int)), this, SLOT(TyslotSlider_DoubleSpinBox()));
    //����ʧ��
    ui.SendButton->setEnabled(false);
    ui.ZeroButton->setEnabled(false);
    ui.PIDCheckBox->setEnabled(false);
    ui.GravityCheckBox->setEnabled(false);
    ui.tabWidget->setEnabled(false);
    //������Ĭ��ѡ�����������460800
    ui.BaudrateBox->setCurrentIndex(2);
    //��ʼ����������
    dspctrl.DSPControlInit();
    //��ʼ��ѭ�����ͱ�־λ
    runflag = 0;
}

// ���մ�������
void SerialPortControlWindow::serialPort_readyRead()
{
    //�ӽ��ջ������ж�ȡ����
    QByteArray buffer = serial.readAll();
}

// ���������������£��������еĶ˿�
void SerialPortControlWindow::on_SearchButton_clicked()
{
    ui.PortNameBox->clear();
    //ͨ��QSerialPortInfo���ҿ��ô���
    foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
    {
        ui.PortNameBox->addItem(info.portName());
    }
}

// ���ڴ򿪰������£����ô��ڲ����򿪴���
void SerialPortControlWindow::on_OpenButton_clicked()
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
        //����ʹ��
        ui.SendButton->setEnabled(true);
        ui.ZeroButton->setEnabled(true);
        ui.PIDCheckBox->setEnabled(true);
        ui.GravityCheckBox->setEnabled(true);
        ui.tabWidget->setEnabled(true);
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
        //����ʧ��
        ui.SendButton->setEnabled(false);
        ui.ZeroButton->setEnabled(false);
        ui.PIDCheckBox->setEnabled(false);
        ui.GravityCheckBox->setEnabled(false);
        ui.tabWidget->setEnabled(false);
        //���߳�ѭ��ֹͣ
        runflag = 0;
        ui.TxSinSlider->setValue(0);
        ui.TySinSlider->setValue(0);
    }
}

// ���Ͱ������£����������������
void SerialPortControlWindow::on_SendButton_clicked()
{
    double Tx = ui.TxdoubleSpinBox->value();
    double Ty = ui.TydoubleSpinBox->value();
    double Ax = ui.AxdoubleSpinBox->value();
    double Ay = ui.AydoubleSpinBox->value();
    double Wx = ui.WxdoubleSpinBox->value();
    double Wy = ui.WydoubleSpinBox->value();
    dspctrl.DSPControlUpdata(Tx, Ty, Ax, Ay, Wx, Wy);
    this->SerialPortSend();
    this->SerialPortSend();
    if (ui.tabWidget->currentIndex() == 1)
    {
        runflag = 1;
        start = clock();
        run();
    }
    else
    {
        runflag = 0;
        ui.TxSinSlider->setValue(0);
        ui.TySinSlider->setValue(0);
    }
}

// ���㰴�����£��������㲢�������
void SerialPortControlWindow::on_ZeroButton_clicked()
{
    runflag = 0;
    ui.TxSinSlider->setValue(0);
    ui.TySinSlider->setValue(0);
    ui.TxdoubleSpinBox->setValue(0.0);
    ui.TydoubleSpinBox->setValue(0.0);
    ui.AxdoubleSpinBox->setValue(0.0);
    ui.AydoubleSpinBox->setValue(0.0);
    ui.WxdoubleSpinBox->setValue(0.0);
    ui.WydoubleSpinBox->setValue(0.0);
    dspctrl.DSPControlClear();
    this->SerialPortSend();
    this->SerialPortSend();
}

// TxDoubleSpinBox�䶯ʱ��TxSlider�ı�
void SerialPortControlWindow::TxslotDoubleSpinBox_Slider()
{
    ui.TxSlider->setValue((int)(ui.TxdoubleSpinBox->value() * 100));
}

// TxSlider�䶯ʱ��TxDoubleSpinBox�ı�
void SerialPortControlWindow::TxslotSlider_DoubleSpinBox()
{
    ui.TxdoubleSpinBox->setValue((double)(ui.TxSlider->value()) / 100.0);
}

// TyDoubleSpinBox�䶯ʱ��TySlider�ı�
void SerialPortControlWindow::TyslotDoubleSpinBox_Slider()
{
    ui.TySlider->setValue((int)(ui.TydoubleSpinBox->value() * 100));
}

// TySlider�䶯ʱ��TyDoubleSpinBox�ı�
void SerialPortControlWindow::TyslotSlider_DoubleSpinBox()
{
    ui.TydoubleSpinBox->setValue((double)(ui.TySlider->value()) / 100.0);
}

// ���������������
void SerialPortControlWindow::SerialPortSend()
{
    for (int i = 0; i < dspctrl.controlBits.size(); i++)
    {
        if (ui.PIDCheckBox->isChecked())
            dspctrl.controlBits[15] |= 0x0001;
        if (ui.GravityCheckBox->isChecked())
            dspctrl.controlBits[15] |= 0x0002;
        if (ui.tabWidget->currentIndex() == 0)
<<<<<<< HEAD
        {
            dspctrl.controlBits[15] |= 0x0004; 
            dspctrl.controlBits[15] &= 0xFFF7;
        }
        else if (ui.tabWidget->currentIndex() == 1)
        {
            dspctrl.controlBits[15] |= 0x0008;
            dspctrl.controlBits[15] &= 0xFFFB;
        }
=======
            dspctrl.controlBits[15] |= 0x0004;
        else if (ui.tabWidget->currentIndex() == 1)
            dspctrl.controlBits[15] |= 0x0008;
>>>>>>> c42466e7046073a892e97b42d3755fd5a2314960
        QByteArray data;
        data.push_back(dspctrl.controlBits[i]);
        serial.write(data);
    }
}

// ���߳���������
void SerialPortControlWindow::run()
{
    QtConcurrent::run(this, &SerialPortControlWindow::loop);
}

// ѭ������
void SerialPortControlWindow::loop()
{
    double Ax = 1000 * ui.AxdoubleSpinBox->value();
    double Ay = 1000 * ui.AydoubleSpinBox->value();
    double wx = ui.WxdoubleSpinBox->value();
    double wy = ui.WydoubleSpinBox->value();
    double t;
    clock_t now;
    while (runflag)
    {
        now = clock();
        t = 0.001 * (now - start);
        int valuex = Ax * sin(wx * t);
        int valuey = Ay * sin(wy * t);
        ui.TxSinSlider->setValue(valuex);
        ui.TySinSlider->setValue(valuey);
    }
}