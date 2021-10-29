#include "SerialPortControlWindow.h"
#include <QMessageBox>
#pragma execution_character_set("UTF-8")

// 初始化类对象
SerialPortControlWindow::SerialPortControlWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //连接信号和槽
    QObject::connect(&serial, &QSerialPort::readyRead, this, &SerialPortControlWindow::serialPort_readyRead);
    connect(ui.TxdoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(TxslotDoubleSpinBox_Slider()));
    connect(ui.TxSlider, SIGNAL(valueChanged(int)), this, SLOT(TxslotSlider_DoubleSpinBox()));
    connect(ui.TydoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(TyslotDoubleSpinBox_Slider()));
    connect(ui.TySlider, SIGNAL(valueChanged(int)), this, SLOT(TyslotSlider_DoubleSpinBox()));
    //按键失能
    ui.SendButton->setEnabled(false);
    ui.ZeroButton->setEnabled(false);
    ui.PIDCheckBox->setEnabled(false);
    ui.GravityCheckBox->setEnabled(false);
    ui.tabWidget->setEnabled(false);
    //波特率默认选择下拉第三项：460800
    ui.BaudrateBox->setCurrentIndex(2);
    //初始化发送数据
    dspctrl.DSPControlInit();
    //初始化循环发送标志位
    runflag = 0;
}

// 接收串口数据
void SerialPortControlWindow::serialPort_readyRead()
{
    //从接收缓冲区中读取数据
    QByteArray buffer = serial.readAll();
}

// 串口搜索按键按下，搜索所有的端口
void SerialPortControlWindow::on_SearchButton_clicked()
{
    ui.PortNameBox->clear();
    //通过QSerialPortInfo查找可用串口
    foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
    {
        ui.PortNameBox->addItem(info.portName());
    }
}

// 串口打开按键按下，配置串口参数打开串口
void SerialPortControlWindow::on_OpenButton_clicked()
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
        //按键使能
        ui.SendButton->setEnabled(true);
        ui.ZeroButton->setEnabled(true);
        ui.PIDCheckBox->setEnabled(true);
        ui.GravityCheckBox->setEnabled(true);
        ui.tabWidget->setEnabled(true);
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
        //按键失能
        ui.SendButton->setEnabled(false);
        ui.ZeroButton->setEnabled(false);
        ui.PIDCheckBox->setEnabled(false);
        ui.GravityCheckBox->setEnabled(false);
        ui.tabWidget->setEnabled(false);
        //多线程循环停止
        runflag = 0;
        ui.TxSinSlider->setValue(0);
        ui.TySinSlider->setValue(0);
    }
}

// 发送按键按下，串口输出力矩数据
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

// 清零按键按下，力矩清零并输出数据
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

// TxDoubleSpinBox变动时，TxSlider改变
void SerialPortControlWindow::TxslotDoubleSpinBox_Slider()
{
    ui.TxSlider->setValue((int)(ui.TxdoubleSpinBox->value() * 100));
}

// TxSlider变动时，TxDoubleSpinBox改变
void SerialPortControlWindow::TxslotSlider_DoubleSpinBox()
{
    ui.TxdoubleSpinBox->setValue((double)(ui.TxSlider->value()) / 100.0);
}

// TyDoubleSpinBox变动时，TySlider改变
void SerialPortControlWindow::TyslotDoubleSpinBox_Slider()
{
    ui.TySlider->setValue((int)(ui.TydoubleSpinBox->value() * 100));
}

// TySlider变动时，TyDoubleSpinBox改变
void SerialPortControlWindow::TyslotSlider_DoubleSpinBox()
{
    ui.TydoubleSpinBox->setValue((double)(ui.TySlider->value()) / 100.0);
}

// 串口输出力矩数据
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

// 多线程启动函数
void SerialPortControlWindow::run()
{
    QtConcurrent::run(this, &SerialPortControlWindow::loop);
}

// 循环函数
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