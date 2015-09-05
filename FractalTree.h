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
#include <QColorDialog>
#include <QSpinBox>

#include "FractalTreeImage.h"
#include "SpinBox.h"

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
    void updateTree();
    void clickedLeafColor();
    void clickedTreeColor();

    void changedValue();
signals:
protected:
    Ui::FractalTree *ui;
    FractalTreeImage *curTree;
    QTimer *timer;
    bool changedTree;
    SpinBox *widthBox;
    SpinBox *heightBox;
    SpinBox *branchesBox;
    SpinBox *depthBox;
    SpinBox *rootBox;

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

    QColor treeColor;
    QColor leafColor;
    void drawTree();
    QString colorToRGBA(const QColor &color);
    void updateStyleSheet();
    void changeColor(QColor &curColor);
};

#endif // FRACTALTREE_H
