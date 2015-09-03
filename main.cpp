#include "FractalTree.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FractalTree w;
    w.show();

    return a.exec();
}
