#include "SpinBox.h"

SpinBox::SpinBox(QWidget *parent) :
    QSpinBox(parent)
{
}

void SpinBox::stepBy(int steps)
{
    QSpinBox::stepBy(steps);
    emit newValue();
}

void SpinBox::focusOutEvent(QFocusEvent * event)
{
    QSpinBox::focusOutEvent(event);
    emit newValue();
}

void SpinBox::keyReleaseEvent(QKeyEvent * event)
{
    QSpinBox::keyReleaseEvent(event);
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        emit newValue();
}
