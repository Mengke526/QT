#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SerialPortApplication.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class SerialPortApplication : public QMainWindow
{
    Q_OBJECT

public:
    SerialPortApplication(QWidget *parent = Q_NULLPTR);

private:
    Ui::SerialPortApplicationClass ui;
    QSerialPort serial;

private slots:
    void serialPort_readyRead();
    void on_SearchButton_clicked();
    void on_OpenButton_clicked();
    void on_SendButton_clicked();
    void on_ClearButton_clicked();
};
