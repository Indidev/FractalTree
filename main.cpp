#include "FractalTree.h"
#include <QApplication>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    srand48(time(NULL));
    QApplication a(argc, argv);
    FractalTree w;
    w.show();

    return a.exec();
}
