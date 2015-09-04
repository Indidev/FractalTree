#ifndef FRACTALTREE_H
#define FRACTALTREE_H

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QRect>
#include <QFileDialog>
#include <limits>
#include <QValidator>
#include <QTimer>

#include "FractalTreeImage.h"

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
    void changeLeafSize();
private:
    Ui::FractalTree *ui;
    FractalTreeImage *curTree;
    QTimer *timer;
    int leafSize;

    //private class
    class LongValidator : public QValidator {
        QValidator::State validate(QString &str, int &) const {
            bool ok;
            long value = str.toLong(&ok);
            if (ok)
                ok = value <= UINT_MAX;
            return ok || str.isEmpty()?State::Acceptable:State::Invalid;
        }
    };
signals:
protected:
    void drawTree();
};

#endif // FRACTALTREE_H
