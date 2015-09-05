#include "ExtendedButton.h"

ExtendedButton::ExtendedButton(QWidget *parent) : QPushButton(parent)
{
    this->setMouseTracking(true);
}

ExtendedButton::ExtendedButton(const QString &text, QWidget *parent) : QPushButton(text, parent)
{
    this->setMouseTracking(true);
}

void ExtendedButton::mousePressEvent(QMouseEvent *event)
{
    (void) event; //ignore event
}

void ExtendedButton::mouseMoveEvent(QMouseEvent *event)
{
    (void) event; //ignore event
    emit hover();
}

void ExtendedButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (rect().contains(event->pos())) {
        if (event->button() == Qt::RightButton)
            emit rightClicked();
        else if(event->button() == Qt::LeftButton)
            emit leftClicked();
        else if (event->button() == Qt::MiddleButton)
            emit middleClicked();

        emit clicked();
    }
}
