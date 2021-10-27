#include "SerialPortApplication.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialPortApplication w;
    w.show();
    return a.exec();
}
