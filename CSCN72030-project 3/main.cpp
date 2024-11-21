#include <iostream>
#include "stdafx.h"
#include "Car.h"
#include "Steering.h"
#include <QtWidgets/QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Car w;
    w.show();
    return a.exec();
}

/*

steps to add new class ui
right click project, add->new item,
qtwidgets class, next, set base class to widget
open filename.ui
design your file
open car.ui
drag and drop new widget
right click->promote
enter class name, add, close, save file

NOTE: the class widget will not appear in car.ui editor, it will show up when you run the program and may require some resizing
*/