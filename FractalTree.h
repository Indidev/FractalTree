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
#include <QSignalMapper>

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
    void clickedLeafColor(int index);
    void clickedTreeColor();

    void changedValue();
    void updateColor();
    void pushedAddColorButton();
signals:
protected:
    Ui::FractalTree *ui;
    FractalTreeImage *curTree;
    QTimer *timer;
    SpinBox *widthBox;
    SpinBox *heightBox;
    SpinBox *branchesBox;
    SpinBox *depthBox;
    SpinBox *rootBox;
    QColor *colorPtr;
    QPushButton *addColorButton;

    bool changedTree;

    QColorDialog colorDialog;

    QColor treeColor;
    QList<QColor> leafColors;
    QList<QPushButton *> leafColorButtons;
    QSignalMapper leafColorMapper;

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
    void drawTree();
    QString colorToRGBA(const QColor &color = QColor(0, 198, 0, 200));
    void updateStyleSheet();
    void changeColor(QColor &curColor);
    void addLeafColorButton(QColor color = QColor(0, 198, 0, 200));
    void addAddColorButton();
};

#endif // FRACTALTREE_H
