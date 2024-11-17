#include <QApplication>
#include <QWidget>
#include "interface.h"


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    Interface w;
    w.setWindowTitle("ROBOTS HANDS DETECTION");
    w.show();
    return a.exec();
}