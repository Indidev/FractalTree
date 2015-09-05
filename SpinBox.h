#ifndef SPINBOX_H
#define SPINBOX_H

#include <QSpinBox>
#include <QWheelEvent>

class SpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit SpinBox(QWidget *parent = 0);

    void stepBy(int steps);
signals:
    void newValue();

public slots:

protected:
    void focusOutEvent(QFocusEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // SPINBOX_H
