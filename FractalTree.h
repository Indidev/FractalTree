#ifndef FRACTALTREE_H
#define FRACTALTREE_H

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QRect>
#include <QFileDialog>
#include <limits>
#include <QValidator>

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
private:
    Ui::FractalTree *ui;
    FractalTreeImage *curTree;

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
};

#endif // FRACTALTREE_H
