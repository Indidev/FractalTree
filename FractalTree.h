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
#include <QMenu>
#include <QCursor>
#include <QErrorMessage>

#include "FractalTreeImage.h"
#include "SpinBox.h"
#include "ExtendedButton.h"

#include <iostream>

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
    void updateTree();
    void clickedLeafColor(int index);
    void clickedTreeColor();

    void changedValue();
    void updateColor();
    void pushedAddColorButton();
    void rightClickedColor(int index);
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
    QSignalMapper leafColorLCMapper;
    QSignalMapper leafColorRCMapper;

    //private class
    class SeedValidator : public QValidator {
        QValidator::State validate(QString &str, int &) const {
            if (str.isEmpty() || str.startsWith("#"))
                return State::Acceptable;
            bool ok;
            long value = str.toLong(&ok);
            if (ok)
                ok = value <= UINT_MAX;
            return ok?State::Acceptable:State::Invalid;
        }
    };
    void drawTree();
    QString colorToRGBA(const QColor &color = QColor(0, 198, 0, 200));
    void updateStyleSheet();
    void changeColor(QColor &curColor);
    void addLeafColor(QColor color = QColor(0, 198, 0, 200));
    void addAddColorButton();
    void deleteLeafColor(int index);
    bool checkHashList(QStringList list);
    QColor getColorFromHash(QString hash);
};

#endif // FRACTALTREE_H
