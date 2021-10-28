#include "SerialPortControlWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialPortControlWindow w;
    w.show();
    return a.exec();
}
