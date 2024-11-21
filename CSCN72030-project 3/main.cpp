#include "stdafx.h"
#include "Car.h"
#include "Steering.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Car w;
    w.show();
    return a.exec();
}
