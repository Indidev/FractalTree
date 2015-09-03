#ifndef FRACTALTREE_H
#define FRACTALTREE_H

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QRect>
#include <QFileDialog>
#include "FractalTreeImage.h"

#include "iostream"

using namespace std;

namespace Ui {
class FractalTree;
}

class FractalTree : public QMainWindow
{
    Q_OBJECT

public:
    explicit FractalTree(QWidget *parent = 0);
    ~FractalTree();

public slots:
    void render();
    void save();
private:
    Ui::FractalTree *ui;
    QImage *curTree;
};

#endif // FRACTALTREE_H
