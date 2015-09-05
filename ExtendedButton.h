#ifndef EXTENDEDBUTTON_H
#define EXTENDEDBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QMouseEvent>

#include <iostream>
using namespace std;

class ExtendedButton : public QPushButton
{
    Q_OBJECT
public:
    ExtendedButton(QWidget *parent = 0);
    ExtendedButton(const QString &text, QWidget *parent = 0);

private slots:
    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void leftClicked();
    void rightClicked();
    void middleClicked();
    void hover();
};

#endif // EXTENDEDBUTTON_H
